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
#include "cprogressdelegate.h"
#include <QApplication>

CProgressDelegate::CProgressDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void CProgressDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int progress = static_cast<int>(index.data().toDouble() * 100.0);

    QStyleOptionProgressBar progressBarOption;
    progressBarOption.rect = option.rect;
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.progress = progress;
    progressBarOption.text = QString::number(progress) + "%";
    progressBarOption.textVisible = true;
    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}
