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
#ifndef CSTATUSWIDGET_H
#define CSTATUSWIDGET_H

#include <QPushButton>
class CVk;
class CIAmReply;

class CStatusWidget : public QPushButton
{
    Q_OBJECT
public:
    explicit CStatusWidget(CVk *vk, QWidget *parent = 0);

public slots:
    void setOnline();
    void setOffline();

private slots:
    void slot_accessTokenChanged(const QString &accessToken);
    void slot_iamreply_finished();

private:
    void setYouAre(const QString &text);
    void setAnonymouse();

private:
    CIAmReply *m_iAmReply;
    CVk *m_vk;

};

#endif // CSTATUSWIDGET_H
