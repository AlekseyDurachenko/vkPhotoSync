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
#ifndef CALBUMREPLY_H
#define CALBUMREPLY_H

#include <QDateTime>
#include <QStringList>
#include "cvk.h"

class CAlbumReply : public QObject
{
    Q_OBJECT
public:
    explicit CAlbumReply(CVk *vk, QObject *parent = 0);

    void setOwnerId(int ownerId);
    void setAlbumId(int albumId);
    inline int ownerId() const;
    inline int albumId() const;
    inline const QString &ownerName() const;
    inline const QString &albumName() const;
    inline const QDateTime &dateTime() const;
    inline const QStringList &photoList() const;
    inline bool isGroup() const; // is owner a group?

    inline bool hasError() const;
    inline const QString &errorString() const;
    inline bool isAborted() const;

signals:
    void finished();

public slots:
    void start();
    void abort();

private slots:
    void reply_finished();

protected:
    void execute(const QString &code);
    void processResponse(const QVariant &response);

private:
    int m_ownerId;
    int m_albumId;
    QString m_ownerName;
    QString m_albumName;
    QDateTime m_dateTime;
    QStringList m_photoList;
    bool m_isGroup;

    bool m_aborted;
    bool m_hasError;
    QString m_errorString;

    CVk *m_vk;
    QNetworkReply *m_reply;

};

int CAlbumReply::ownerId() const
{
    return m_ownerId;
}

int CAlbumReply::albumId() const
{
    return m_albumId;
}

const QString &CAlbumReply::ownerName() const
{
    return m_ownerName;
}

const QString &CAlbumReply::albumName() const
{
    return m_albumName;
}

const QDateTime &CAlbumReply::dateTime() const
{
    return m_dateTime;
}

const QStringList &CAlbumReply::photoList() const
{
    return m_photoList;
}

bool CAlbumReply::isGroup() const
{
    return m_isGroup;
}

bool CAlbumReply::hasError() const
{
    return m_hasError;
}

const QString &CAlbumReply::errorString() const
{
    return m_errorString;
}

bool CAlbumReply::isAborted() const
{
    return m_aborted;
}

#endif // CALBUMREPLY_H
