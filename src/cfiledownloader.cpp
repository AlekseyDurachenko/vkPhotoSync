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
#include "cfiledownloader.h"
#include "consts.h"
#include "cvk.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileInfo>
#include <QTimer>
#include <QDir>

CFileDownloader::CFileDownloader(CVk *vk, QObject *parent)
    : QObject(parent)
{
    m_url = QUrl();
    m_fileName = QString();

    m_vk = vk;
    m_reply = 0;
    m_file = new QFile(this);

    m_isAborted = false;
    m_hasError = false;
    m_errorString = QString();
}

CFileDownloader::~CFileDownloader()
{
    if (m_file->isOpen())
    {
        m_file->close();
    }

    // temporary files after application quitting shouln't be exists
    if (!m_fileName.isEmpty() && QFile(tmpFileName(m_fileName)).exists())
    {
        QFile::remove(tmpFileName(m_fileName));
    }
}

void CFileDownloader::setUrl(const QUrl &url)
{
    m_url = url;
}

void CFileDownloader::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void CFileDownloader::start()
{
    m_isAborted = false;
    m_hasError = false;
    m_errorString = QString();

    if (QFile(m_fileName).exists())
    {
        emit finished();
    }
    else
    {
        // start download the file
        QNetworkRequest request;
        request.setUrl(m_url);
        request.setRawHeader("User-Agent", g_userAgent);

        m_reply = m_vk->dlNetwork()->get(request);
        connect(m_reply, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
        connect(m_reply, SIGNAL(finished()), this, SLOT(slot_finished()));
        connect(this, SIGNAL(destroyed()), m_reply, SLOT(deleteLater()));
    }
}

void CFileDownloader::abort()
{
    if (m_reply)
    {
        m_reply->abort();
    }
}

void CFileDownloader::slot_readyRead()
{
    if (!m_file->isOpen())
    {
        // we should open the temprary file on first image data received
        m_file->setFileName(tmpFileName(m_fileName));
        if (!m_file->open(QIODevice::WriteOnly))
        {
            // can't open the file: this situation will resolved in slot_finished()
            m_reply->abort();
            return;
        }
    }

    qint64 bytesAvailable = m_reply->bytesAvailable();
    if (m_file->write(m_reply->read(bytesAvailable)) != bytesAvailable)
    {
        // can't write bytes to file: this situation will resolved in slot_finished()
        m_reply->abort();
    }
}

void CFileDownloader::slot_finished()
{
    if (m_file->isOpen())
    {
        m_file->close();
    }

    // if no error then copy temporary to destinate and emit success
    if (m_reply->error() == QNetworkReply::NoError && m_file->error() == QFile::NoError)
    {
        if (QFile(tmpFileName(m_fileName)).exists() && !QFile(m_fileName).exists())
        {
            QFile::rename(tmpFileName(m_fileName), m_fileName);
        }
        // if temporary file is not exists we suppouse that the file on the server side
        // is empty, and will not try to get the file in the future
        // just mark the file as downloaded
        emit finished();
    }
    else
    {
        if (QFile(tmpFileName(m_fileName)).exists())
        {
            QFile::remove(tmpFileName(m_fileName));
        }

        // aborted by user
        if (m_reply->error() == QNetworkReply::OperationCanceledError && m_file->error() == QFile::NoError)
        {
            m_isAborted = true;
            emit finished();
        }
        // may be network problem?
        else if (m_reply->error() != QNetworkReply::NoError && m_file->error() == QFile::NoError)
        {
            QTimer::singleShot(m_vk->retryNetworkInterval(), this, SLOT(start()));
        }
        else
        {
            m_hasError = true;
            // IO problem
            if (m_file->error() != QFile::NoError)
            {
                m_errorString = m_file->errorString();
            }
            // network problem
            else if(m_reply->error() != QNetworkReply::NoError)
            {
                m_errorString = m_reply->errorString();
            }

            emit finished();
        }
    }

    // cleanup
    if (m_reply)
    {
        m_reply->deleteLater();
        m_reply = 0;
    }    

    m_file->unsetError();
}

QString CFileDownloader::tmpFileName(const QString &fileName)
{
    return fileName + "~";
}
