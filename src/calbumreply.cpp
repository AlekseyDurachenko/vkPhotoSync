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
    QFile file(":/vkscript/get_photos.vks");
    file.open(QIODevice::ReadOnly);
    execute(QString::fromUtf8(file.readAll()).arg(m_ownerId).arg(m_albumId));
}

void CAlbumReply::processResponse(const QVariant &response)
{
    QVariantMap responseMap = response.toMap();
    m_ownerName = QString::fromUtf8(responseMap.value("owner_name").toByteArray());
    m_albumName = QString::fromUtf8(responseMap.value("album_name").toByteArray());
    m_dateTime = QDateTime::fromTime_t(responseMap.value("updated_time").toInt());
    m_isGroup = responseMap.value("is_goup").toBool();

    m_photoList.clear();
    foreach (const QVariant &photoLink, responseMap.value("photo_list").toList())
    {
        m_photoList.push_back(photoLink.toString());
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
                QTimer::singleShot(m_vk->retryApiInterval(), this, SLOT(start()));
                isRetry = true;
            }
            // the another error is fatal, no chance for correct them
            else
            {
                QString errorMsg = replyVariant.value("error").toMap().value("error_msg").toString();
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
        QTimer::singleShot(m_vk->retryNetworkInterval(), this, SLOT(start()));
        isRetry = true;
    }

    m_reply->deleteLater();
    m_reply = 0;

    // if timer is active
    if (!isRetry)
    {
        emit finished();
    }
}
