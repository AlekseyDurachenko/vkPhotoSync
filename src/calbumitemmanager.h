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
#ifndef CALBUMITEMMANAGER_H
#define CALBUMITEMMANAGER_H

#include <QObject>
#include "calbumitem.h"
#include "cvk.h"
#include "calbumreply.h"
#include "cfiledownloader.h"

class CAlbumItemManager : public QObject
{
    Q_OBJECT
public:
    explicit CAlbumItemManager(CVk *vk, QObject *parent = 0);
    ~CAlbumItemManager();

    inline int count() const;
    bool contains(int ownerId, int albumId) const;
    int indexOf(int ownerId, int albumId) const;
    void insert(CAlbumItem *albumItem);
    void remove(int index);
    void increasePriority(int first, int last);
    void decreasePriority(int first, int last);
    CAlbumItem *value(int index) const;
    void start(int index);
    void pause(int index);
    void update(int index);

signals:
    void removed(int first, int last);
    void inserted(int first, int last);
    void moved(int first, int last, int dest);
    void dataChanged(int index);

private slots:
    void processNextAlbum();
    void processNextDownload();
    void albumReply_finished();
    void fileDownloader_finished();

private:
    QList<CAlbumItem *> m_albumItemList;
    CVk *m_vk;

    // state machine for album information fetching
    CAlbumReply *m_albumReply;
    QSet<CFileDownloader*> m_fileDownloaders;
};

int CAlbumItemManager::count() const
{
    return m_albumItemList.count();
}

#endif // CALBUMITEMMANAGER_H
