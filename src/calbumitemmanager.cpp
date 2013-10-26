// Copyright 2013, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "calbumitemmanager.h"
#include "calbumitemmodel.h"
#include <QDebug>
#include <QDir>
#include <QUrl>

CAlbumItemManager::CAlbumItemManager(CVk *vk, QObject *parent)
    : QObject(parent)
{
    m_vk = vk;
    m_albumReply = 0;
}

CAlbumItemManager::~CAlbumItemManager()
{
}

bool CAlbumItemManager::contains(int ownerId, int albumId) const
{
    for (int i = 0; i < m_albumItemList.count(); ++i)
    {
        if (m_albumItemList[i]->albumId() == albumId && m_albumItemList[i]->ownerId() == ownerId)
        {
            return true;
        }
    }

    return false;
}

int CAlbumItemManager::indexOf(int ownerId, int albumId) const
{
    for (int i = 0; i < m_albumItemList.count(); ++i)
    {
        if (m_albumItemList[i]->albumId() == albumId && m_albumItemList[i]->ownerId() == ownerId)
        {
            return i;
        }
    }

    return -1;
}

void CAlbumItemManager::insert(CAlbumItem *albumItem)
{
    m_albumItemList.push_back(albumItem);
    emit inserted(m_albumItemList.count()-1, m_albumItemList.count()-1);

    if (albumItem->state() == CAlbumItem::Updating)
    {
        processNextAlbum();
    }
    else if (albumItem->state() == CAlbumItem::Downloading)
    {
        processNextDownload();
    }
}

void CAlbumItemManager::remove(int index)
{
    delete m_albumItemList.takeAt(index);
    emit removed(index, index);
}

void CAlbumItemManager::increasePriority(int first, int last)
{
    if (first > 0)
    {
        for (int index = first; index <= last; ++index)
        {
            m_albumItemList.swap(index, index-1);
        }
        emit moved(first, last, first - 1);
    }
}

void CAlbumItemManager::decreasePriority(int first, int last)
{
    if (last < m_albumItemList.count() - 1)
    {
        for (int index = last; index >= first; --index)
        {
            m_albumItemList.swap(index, index+1);
        }
        // hack! because segfault
        emit moved(last+1, last+1, first);
    }
}

CAlbumItem *CAlbumItemManager::value(int index) const
{
    return m_albumItemList[index];
}

void CAlbumItemManager::start(int index)
{
    m_albumItemList.value(index)->setState(CAlbumItem::Updating);
    emit dataChanged(index);

    processNextAlbum();
}

void CAlbumItemManager::pause(int index)
{
    CAlbumItem *album = m_albumItemList.value(index);
    album->setState(CAlbumItem::Inactive);
    foreach (CFileDownloader *downloader, m_fileDownloaders)
    {
        if (album->ownerId() == downloader->property("owner_id").toInt() &&
                album->albumId() == downloader->property("album_id").toInt())
        {
            downloader->abort();
        }
    }
    emit dataChanged(index);
}

void CAlbumItemManager::processNextAlbum()
{
    // the other album is processing, this slot will be called anyway
    if (m_albumReply == 0)
    {
        for (int index = 0; index < count(); ++index)
        {
            CAlbumItem *album = m_albumItemList.value(index);
            if (album->state() == CAlbumItem::Updating)
            {
                m_albumReply = new CAlbumReply(m_vk, this);
                m_albumReply->setOwnerId(album->ownerId());
                m_albumReply->setAlbumId(album->albumId());
                connect(m_albumReply, SIGNAL(finished()), this, SLOT(albumReply_finished()), Qt::QueuedConnection);
                m_albumReply->start();

                return;
            }
        }
    }
}

void CAlbumItemManager::processNextDownload()
{
    // the other download is processing, this slot will be called anyway
    bool nextDownloadExists = true;
    while (m_fileDownloaders.count() < m_vk->maximumDownloads() && nextDownloadExists)
    {
        nextDownloadExists = false;
        for (int index = 0; index < count(); ++index)
        {
            CAlbumItem *album = m_albumItemList.value(index);
            if (album->state() == CAlbumItem::Downloading)
            {
                QString url = album->nextUnprocessedPhoto();
                QString fileName = album->outputDirectory() + QDir::separator() + QUrl(url).path().replace("/", "_");

                CFileDownloader *downloader = new CFileDownloader(m_vk, this);
                downloader->setProperty("owner_id", album->ownerId());
                downloader->setProperty("album_id", album->albumId());
                downloader->setFileName(fileName);
                downloader->setUrl(url);
                connect(downloader, SIGNAL(finished()), this, SLOT(fileDownloader_finished()), Qt::QueuedConnection);
                downloader->start();

                m_fileDownloaders.insert(downloader);
                nextDownloadExists = true;
                break;
            }
        }
    }
}

void CAlbumItemManager::albumReply_finished()
{
    int index = indexOf(m_albumReply->ownerId(), m_albumReply->albumId());

    // the albumItem may be removed while reply is executed
    if (index != -1)
    {
        CAlbumItem *item = value(index);
        if (m_albumReply->hasError() == false)
        {
            item->setAlbumName(m_albumReply->albumName());
            item->setOwnerName(m_albumReply->ownerName());
            item->setDateTime(m_albumReply->dateTime());
            item->setPhotoQueue(m_albumReply->photoList());
            item->setState(CAlbumItem::Downloading);
            item->setCount(m_albumReply->photoList().count());

            processNextDownload();
        }
        else
        {
            item->setDateTime(QDateTime());
            item->setState(CAlbumItem::Failed);
            item->setPhotoQueue(QStringList());
            item->setCount(0);
            item->setErrorString(m_albumReply->errorString());
        }
        emit dataChanged(index);
    }

    m_albumReply->deleteLater();
    m_albumReply = 0;

    processNextAlbum();
}

void CAlbumItemManager::fileDownloader_finished()
{
    CFileDownloader *downloader = qobject_cast<CFileDownloader*>(sender());
    int index = indexOf(downloader->property("owner_id").toInt(), downloader->property("album_id").toInt());

    // the albumItem may be removed while file is downloading
    if (index != -1)
    {
        CAlbumItem *album = m_albumItemList.value(index);

        if (downloader->hasError())
        {
            album->setState(CAlbumItem::Failed);
            album->setErrorString(downloader->errorString());
            emit dataChanged(index);
        }
        else if (!downloader->isAborted())
        {
            album->markPhotoAsProcessed(downloader->url().toString());
            if (album->photoQueueSize() == 0)
            {
                album->setState(CAlbumItem::Success);
            }
            emit dataChanged(index);
        }
    }

    downloader->deleteLater();
    m_fileDownloaders.remove(downloader);

    processNextDownload();
}
