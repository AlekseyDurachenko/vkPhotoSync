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
#ifndef CALBUMITEM_H
#define CALBUMITEM_H

#include <QStringList>
#include <QDateTime>
#include <QHash>
#include <QSet>

class CAlbumItem
{
public:
    enum State { Inactive, Updating, Downloading, Success, Failed };

    explicit CAlbumItem();

    // basic
    inline int ownerId() const;
    inline int albumId() const;
    inline const QString &ownerName() const;
    inline const QString &albumName() const;
    inline const QString &outputDirectory() const;
    inline const QDateTime &dateTime() const;
    inline int count();
    void setOwnerId(int ownerId);
    void setAlbumId(int albumId);
    void setOwnerName(const QString &ownerName);
    void setAlbumName(const QString &albumName);
    void setOutputDirectory(const QString &outputDirectory);
    void setDateTime(const QDateTime &dateTime);
    void setCount(int count);

    // state
    inline State state() const;
    inline const QString &errorString() const;
    void setState(State state);
    void setErrorString(const QString &errorString);

    // photo queue
    QStringList photoQueue() const;
    void setPhotoQueue(const QStringList &photoList);
    inline int photoQueueSize() const;
    QString nextUnprocessedPhoto();
    void markPhotoAsProcessed(const QString &photoUrl);

private:
    int m_ownerId;
    int m_albumId;
    QString m_ownerName;
    QString m_albumName;
    QString m_outputDirectory;
    QDateTime m_dateTime;
    int m_count;

    State m_state;
    QString m_errorString;

    QSet<QString> m_photoQueue;
    QSet<QString> m_photoUnprocessed;

};

int CAlbumItem::ownerId() const
{
    return m_ownerId;
}

int CAlbumItem::albumId() const
{
    return m_albumId;
}

const QString &CAlbumItem::ownerName() const
{
    return m_ownerName;
}

const QString &CAlbumItem::albumName() const
{
    return m_albumName;
}

const QString &CAlbumItem::outputDirectory() const
{
    return m_outputDirectory;
}

const QDateTime &CAlbumItem::dateTime() const
{
    return m_dateTime;
}

CAlbumItem::State CAlbumItem::state() const
{
    return m_state;
}

const QString &CAlbumItem::errorString() const
{
    return m_errorString;
}

int CAlbumItem::count()
{
    return m_count;
}

int CAlbumItem::photoQueueSize() const
{
    return m_photoQueue.size();
}

#endif // CALBUMITEM_H
