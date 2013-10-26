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

CAlbumReply::CAlbumReply(CVk *vk, QObject *parent)
    : CBaseReply(vk, parent)
{
    m_ownerId = 0;
    m_albumId = 0;
    m_ownerName = QString();
    m_albumName = QString();
    m_dateTime = QDateTime();
    m_photoList = QStringList();
    m_isGroup = false;
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
