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
#include "cstatuswidget.h"
#include "ciamreply.h"
#include "cvk.h"

CStatusWidget::CStatusWidget(CVk *vk, QWidget *parent)
    : QPushButton(parent)
{
    // button settings
    setFlat(true);

    // init
    m_vk = vk;
    m_iAmReply = 0;
    setAnonymouse();

    connect(m_vk, SIGNAL(accessTokenChanged(QString)), this, SLOT(slot_accessTokenChanged(QString)));
}

void CStatusWidget::setOnline()
{
    setIcon(QIcon(":/icons/online.png"));
}

void CStatusWidget::setOffline()
{
    setIcon(QIcon(":/icons/offline.png"));
}

void CStatusWidget::slot_accessTokenChanged(const QString &accessToken)
{
    if (m_iAmReply)
    {
        m_iAmReply->abort();
        m_iAmReply = 0;
    }

    if (!accessToken.isEmpty())
    {
        m_iAmReply = new CIAmReply(m_vk, this);
        connect(m_iAmReply, SIGNAL(finished()), this, SLOT(slot_iamreply_finished()));
        m_iAmReply->start();
    }
    else
    {
        setAnonymouse();
    }
}

void CStatusWidget::slot_iamreply_finished()
{
    if (m_iAmReply->hasError())
    {
        setAnonymouse();
        setToolTip(m_iAmReply->errorString());
    }
    else
    {
        setYouAre(m_iAmReply->myName());
        setToolTip(QString());
    }

    m_iAmReply->deleteLater();
    m_iAmReply = 0;
}

void CStatusWidget::setYouAre(const QString &text)
{
    setText(tr("You are %1").arg(text));
    setIcon(QIcon(":/icons/online.png"));
}

void CStatusWidget::setAnonymouse()
{
    setYouAre(tr("Anonymouse"));
    setIcon(QIcon(":/icons/anonymouse.png"));
}
