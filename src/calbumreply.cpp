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
#include "calbumreply.h"
#include <QFile>
#include <QVariantMap>
#include <QNetworkReply>
#include <QTimer>
#include <json.h>
#include "consts.h"
#include <QDebug>

CAlbumReply::CAlbumReply(CVk *vk, QObject *parent)
    : QObject(parent)
{
    m_ownerId = 0;
    m_albumId = 0;
    m_ownerName = QString();
    m_albumName = QString();
    m_dateTime = QDateTime();
    m_photoList = QStringList();
    m_isGroup = false;

    m_aborted = false;
    m_hasError = false;
    m_errorString = QString();

    m_vk = vk;
    m_reply = 0;
    m_state = None;
}

void CAlbumReply::setOwnerId(int ownerId)
{
    m_ownerId = ownerId;
}

void CAlbumReply::setAlbumId(int albumId)
{
    m_albumId = albumId;
}

void CAlbumReply::start()
{
    abort();
    m_aborted = false;
    m_state = None;
    next_state();
}

void CAlbumReply::processResponse(const QVariant &response)
{
    if (m_state == WaitOwnerName)
    {
        if (m_isGroup)
        {
            m_ownerName = QString::fromUtf8(response.toList().value(0).toMap().value("name").toByteArray());
        }
        else
        {
            m_ownerName = QString::fromUtf8(response.toList().value(0).toMap().value("first_name").toByteArray()) +
                    " " + QString::fromUtf8(response.toList().value(0).toMap().value("last_name").toByteArray());
        }
    }
    else if (m_state == WaitAlbumName)
    {
        QVariantMap albumMap = response.toMap().value("items").toList().value(0).toMap();
        m_albumName = QString::fromUtf8(albumMap.value("title").toByteArray());
        m_dateTime = QDateTime::fromTime_t(albumMap.value("updated").toInt());
    }
    else if (m_state == WaitPhotoList)
    {
        QList<QVariant> photoList = response.toMap().value("items").toList();
        foreach (QVariant photo, photoList)
        {
            QVariantMap map = photo.toMap();
            if (map.contains("photo_2560"))
            {
                m_photoList.push_back(map.value("photo_2560").toString());
            }
            else if (map.contains("photo_1280"))
            {
                m_photoList.push_back(map.value("photo_1280").toString());
            }
            else if (map.contains("photo_807"))
            {
                m_photoList.push_back(map.value("photo_807").toString());
            }
            else if (map.contains("photo_604"))
            {
                m_photoList.push_back(map.value("photo_604").toString());
            }
            else if (map.contains("photo_130"))
            {
                m_photoList.push_back(map.value("photo_130").toString());
            }
            else if (map.contains("photo_75"))
            {
                m_photoList.push_back(map.value("photo_75").toString());
            }
        }
    }
}

void CAlbumReply::execute(const QString &code)
{
    m_aborted = false;
    m_hasError = false;
    m_errorString = QString();

    QUrl apiUrl = QUrl(QString("https://api.vk.com/method/execute"));
    apiUrl.addQueryItem("v", g_protocolVersion);
    if (!m_vk->accessToken().isEmpty())
        apiUrl.addQueryItem("access_token", m_vk->accessToken());
    apiUrl.addEncodedQueryItem("code", QUrl::toPercentEncoding(code));

    QNetworkRequest request;
    request.setUrl(apiUrl);
    request.setRawHeader("User-Agent", g_userAgent);
    m_reply = m_vk->apiNetwork()->get(request);

    connect(m_reply, SIGNAL(finished()), this, SLOT(reply_finished()));
    connect(this, SIGNAL(destroyed()), m_reply, SLOT(deleteLater()));
}

void CAlbumReply::abort()
{
    if (m_reply)
    {
        m_aborted = true;
        m_reply->abort();
    }
}

void CAlbumReply::next_state()
{
    if (!m_aborted)
    {
        if (m_state == None)
        {
            m_state = WaitOwnerName;
            if (m_ownerId < 0)
            {
                m_isGroup = true;
                ownerAsGroup();
            }
            else
            {
                m_isGroup = false;
                ownerAsUser();
            }
        }
        else if (m_state == WaitOwnerName)
        {
            m_state = WaitAlbumName;
            getAlbumInfo();
        }
        else if (m_state == WaitAlbumName)
        {
            m_state = WaitPhotoList;
            getPhotoList();
        }
        else
        {
            m_state = End;
            emit finished();
        }
    }
}

void CAlbumReply::reply_finished()
{
    bool isRetry = false; // true if we activete the timer

    if (m_reply->error() == QNetworkReply::NoError)
    {
        QVariantMap replyVariant = QtJson::parse(m_reply->readAll()).toMap();
        if (replyVariant.contains("error"))
        {
            int errorCode = replyVariant.value("error").toMap().value("error_code").toInt();

            // "too many request per second" -- sleep 0.5 second and try again
            if (errorCode == 6)
            {
                QTimer::singleShot(m_vk->retryApiInterval(), this, SLOT(next_state()));
                isRetry = true;
                if (m_state == WaitOwnerName)
                {
                    m_state = None;
                }
                else if (m_state == WaitAlbumName)
                {
                    m_state = WaitOwnerName;
                }
                else if (m_state == WaitPhotoList)
                {
                    m_state = WaitAlbumName;
                }
            }
            // the another error is fatal, no chance for correct them
            else
            {
                QString errorMsg = replyVariant.value("error").toMap().value("error_msg").toString();
                m_state = End;
                m_hasError = true;
                m_errorString = QString("%1: %2").arg(errorCode).arg(errorMsg);
            }
        }
        else if (replyVariant.contains("response"))
        {
            // the class heir should process the response
            processResponse(replyVariant.value("response"));
        }
    }
    // may be network problem?
    else if (m_reply->error() != QNetworkReply::OperationCanceledError)
    {
        QTimer::singleShot(m_vk->retryNetworkInterval(), this, SLOT(next_state()));
        isRetry = true;
        if (m_state == WaitOwnerName)
        {
            m_state = None;
        }
        else if (m_state == WaitAlbumName)
        {
            m_state = WaitOwnerName;
        }
        else if (m_state == WaitPhotoList)
        {
            m_state = WaitAlbumName;
        }
    }

    m_reply->deleteLater();
    m_reply = 0;

    if (!isRetry)
    {
        QMetaObject::invokeMethod(this, "next_state", Qt::QueuedConnection);
    }
}

void CAlbumReply::ownerAsGroup()
{
    m_hasError = false;
    m_errorString = QString();

    QUrl apiUrl = QUrl(QString("https://api.vk.com/method/groups.getById"));
    apiUrl.addQueryItem("group_id", QString::number(-m_ownerId));
    apiUrl.addQueryItem("v", g_protocolVersion);
    if (!m_vk->accessToken().isEmpty())
        apiUrl.addQueryItem("access_token", m_vk->accessToken());

    QNetworkRequest request;
    request.setUrl(apiUrl);
    request.setRawHeader("User-Agent", g_userAgent);
    m_reply = m_vk->apiNetwork()->get(request);

    connect(m_reply, SIGNAL(finished()), this, SLOT(reply_finished()));
    connect(this, SIGNAL(destroyed()), m_reply, SLOT(deleteLater()));
}

void CAlbumReply::ownerAsUser()
{
    m_hasError = false;
    m_errorString = QString();

    QUrl apiUrl = QUrl(QString("https://api.vk.com/method/users.get"));
    apiUrl.addQueryItem("user_ids", QString::number(m_ownerId));
    apiUrl.addQueryItem("v", g_protocolVersion);
    if (!m_vk->accessToken().isEmpty())
        apiUrl.addQueryItem("access_token", m_vk->accessToken());

    QNetworkRequest request;
    request.setUrl(apiUrl);
    request.setRawHeader("User-Agent", g_userAgent);
    m_reply = m_vk->apiNetwork()->get(request);

    connect(m_reply, SIGNAL(finished()), this, SLOT(reply_finished()));
    connect(this, SIGNAL(destroyed()), m_reply, SLOT(deleteLater()));
}

void CAlbumReply::getAlbumInfo()
{
    m_hasError = false;
    m_errorString = QString();

    QUrl apiUrl = QUrl(QString("https://api.vk.com/method/photos.getAlbums"));
    apiUrl.addQueryItem("owner_id", QString::number(m_ownerId));
    apiUrl.addQueryItem("album_ids", QString::number(m_albumId));
    apiUrl.addQueryItem("v", g_protocolVersion);
    if (!m_vk->accessToken().isEmpty())
        apiUrl.addQueryItem("access_token", m_vk->accessToken());

    QNetworkRequest request;
    request.setUrl(apiUrl);
    request.setRawHeader("User-Agent", g_userAgent);
    m_reply = m_vk->apiNetwork()->get(request);

    connect(m_reply, SIGNAL(finished()), this, SLOT(reply_finished()));
    connect(this, SIGNAL(destroyed()), m_reply, SLOT(deleteLater()));
}

void CAlbumReply::getPhotoList()
{
    m_hasError = false;
    m_errorString = QString();

    QUrl apiUrl = QUrl(QString("https://api.vk.com/method/photos.get"));
    apiUrl.addQueryItem("owner_id", QString::number(m_ownerId));
    apiUrl.addQueryItem("album_id", QString::number(m_albumId));
    apiUrl.addQueryItem("v", g_protocolVersion);
    if (!m_vk->accessToken().isEmpty())
        apiUrl.addQueryItem("access_token", m_vk->accessToken());

    QNetworkRequest request;
    request.setUrl(apiUrl);
    request.setRawHeader("User-Agent", g_userAgent);
    m_reply = m_vk->apiNetwork()->get(request);

    connect(m_reply, SIGNAL(finished()), this, SLOT(reply_finished()));
    connect(this, SIGNAL(destroyed()), m_reply, SLOT(deleteLater()));
}
