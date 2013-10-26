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
#ifndef CIAMREPLY_H
#define CIAMREPLY_H

#include "cbasereply.h"

class CIAmReply : public CBaseReply
{
    Q_OBJECT
public:
    explicit CIAmReply(CVk *vk, QObject *parent = 0);

    inline const QString &myName() const;

public slots:
    void start();

protected:
    void processResponse(const QVariant &response);

private:
    QString m_myName;

};

const QString &CIAmReply::myName() const
{
    return m_myName;
}

#endif // CIAMREPLY_H
