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
#include "ciamreply.h"
#include <QFile>
#include "json.h"

CIAmReply::CIAmReply(CVk *vk, QObject *parent) :
    CBaseReply(vk, parent)
{
    m_myName = QString();
}

void CIAmReply::start()
{
    QFile file(":/vkscript/who_am_i.vks");
    file.open(QIODevice::ReadOnly);
    execute(QString::fromUtf8(file.readAll()));
}

void CIAmReply::processResponse(const QVariant &response)
{
    m_myName = QString::fromUtf8(response.toByteArray());
}
