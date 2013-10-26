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
#include "calbumitem.h"

CAlbumItem::CAlbumItem()
{
    m_ownerId = 0;
    m_albumId = 0;
    m_ownerName = QString();
    m_albumName = QString();
    m_outputDirectory = QString();
    m_dateTime = QDateTime();
    m_state = Inactive;
    m_count = 0;
}

void CAlbumItem::setOwnerId(int ownerId)
{
    m_ownerId = ownerId;
}

void CAlbumItem::setAlbumId(int albumId)
{
    m_albumId = albumId;
}

void CAlbumItem::setOwnerName(const QString &ownerName)
{
    m_ownerName = ownerName;
}

void CAlbumItem::setAlbumName(const QString &albumName)
{
    m_albumName = albumName;
}

void CAlbumItem::setOutputDirectory(const QString &outputDirectory)
{
    m_outputDirectory = outputDirectory;
}

void CAlbumItem::setDateTime(const QDateTime &dateTime)
{
    m_dateTime = dateTime;
}

void CAlbumItem::setState(CAlbumItem::State state)
{
    m_state = state;
}

void CAlbumItem::setErrorString(const QString &errorString)
{
    m_errorString = errorString;
}

void CAlbumItem::setCount(int count)
{
    m_count = count;
}

QStringList CAlbumItem::photoQueue() const
{
    return m_photoQueue.toList();
}

void CAlbumItem::setPhotoQueue(const QStringList &photoList)
{
    m_photoQueue = QSet<QString>::fromList(photoList);
    m_photoUnprocessed = QSet<QString>::fromList(photoList);
}

void CAlbumItem::markPhotoAsProcessed(const QString &photoUrl)
{
    m_photoQueue.remove(photoUrl);
}

QString CAlbumItem::nextUnprocessedPhoto()
{
    if (m_photoUnprocessed.size())
    {
        const QString &value = *m_photoUnprocessed.constBegin();
        m_photoUnprocessed.remove(value);
        return value;
    }

    return QString();
}
