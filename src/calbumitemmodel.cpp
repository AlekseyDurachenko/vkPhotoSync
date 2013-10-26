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
#include "calbumitemmodel.h"
#include <QIcon>

CAlbumItemModel::CAlbumItemModel(CAlbumItemManager *manager, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_manager = manager;
    connect(m_manager, SIGNAL(inserted(int,int)), this, SLOT(on_manager_inserted(int,int)));
    connect(m_manager, SIGNAL(removed(int,int)), this, SLOT(on_manager_removed(int,int)));
    connect(m_manager, SIGNAL(moved(int,int,int)), this, SLOT(on_manager_moved(int,int,int)));
    connect(m_manager, SIGNAL(dataChanged(int)), this, SLOT(on_manager_dataChanged(int)));
}

int CAlbumItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 7;
}

void CAlbumItemModel::on_manager_removed(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
    endRemoveRows();
}

void CAlbumItemModel::on_manager_inserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
    endInsertRows();
}

void CAlbumItemModel::on_manager_moved(int first, int last, int dest)
{
    beginMoveRows(QModelIndex(), first, last, QModelIndex(), dest);
    endMoveRows();
}

void CAlbumItemModel::on_manager_dataChanged(int index)
{
    emit dataChanged(createIndex(index, 0), createIndex(index, 6));
}

QVariant CAlbumItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || m_manager == 0)
    {
        return QVariant();
    }    

    CAlbumItem *album = m_manager->value(index.row());
    if (role == Qt::ForegroundRole)
    {
        if (album->state() == CAlbumItem::Failed)
        {
            return Qt::darkRed;
        }
        else if (album->state() == CAlbumItem::Downloading || album->state() == CAlbumItem::Updating)
        {
            return Qt::darkGreen;
        }

        return Qt::darkGray;
    }
    else if (role == Qt::DecorationRole && index.column() == 0)
    {
        if (album->state() == CAlbumItem::Failed)
        {
            return QIcon(":/icons/failed.png");
        }
        else if (album->state() == CAlbumItem::Downloading || album->state() == CAlbumItem::Updating)
        {
            return QIcon(":/icons/downloading.png");
        }
        else if (album->state() == CAlbumItem::Success)
        {
            return QIcon(":/icons/downloaded.png");
        }

        return QIcon(":/icons/inactive.png");
    }
    else if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return album->ownerName();
        }
        else if (index.column() == 1)
        {
            return album->albumName();
        }
        else if (index.column() == 2)
        {
            if (album->dateTime().isNull())
            {
                return "-";
            }
            return album->dateTime().toString("yyyy.MM.dd HH:mm");
        }
        else if (index.column() == 3)
        {
            if (album->dateTime().isNull())
            {
                return "-";
            }
            return album->count();
        }
        else if (index.column() == 4)
        {
            if (album->count() > 0)
            {
                return ((album->count() - album->photoQueueSize()) / (1.0 * album->count()));
            }
            if (album->dateTime().isNull())
            {
                return 0.0;
            }
            return 1.0;
        }
        else if (index.column() == 5)
        {
            if (album->state() == CAlbumItem::Failed)
            {
                return album->errorString();
            }
            else if (album->state() == CAlbumItem::Updating)
            {
                return tr("Updating");
            }
            else if (album->state() == CAlbumItem::Downloading)
            {
                return tr("Downloading");
            }
            else if (album->state() == CAlbumItem::Success)
            {
                return tr("Success");
            }

            return tr("Paused");
        }
        else if (index.column() == 6)
        {
            return album->outputDirectory();
        }
    }

    return QVariant();
}

Qt::ItemFlags CAlbumItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant CAlbumItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
        {
            return tr("Owner");
        }
        else if (section == 1)
        {
            return tr("Album");
        }
        else if (section == 2)
        {
            return tr("Updated");
        }
        else if (section == 3)
        {
            return tr("Count");
        }
        else if (section == 4)
        {
            return tr("Progress");
        }
        else if (section == 5)
        {
            return tr("Status");
        }
        else if (section == 6)
        {
            return tr("Download directory");
        }
    }

    return QVariant();
}

QModelIndex CAlbumItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    return createIndex(row, column, 0);
}

QModelIndex CAlbumItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    return QModelIndex();
}

int CAlbumItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || parent.isValid() || m_manager == 0)
    {
        return 0;
    }

    return m_manager->count();
}
