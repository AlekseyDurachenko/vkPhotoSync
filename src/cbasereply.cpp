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
#include "cbasereply.h"
#include "cvk.h"
#include <QNetworkReply>
#include <QTimer>
#include <json.h>

CBaseReply::CBaseReply(CVk *vk, QObject *parent) :
    QObject(parent)
{
    m_aborted = false;
    m_hasError = false;
    m_errorString = QString();

    m_vk = vk;
    m_reply = 0;
}

void CBaseReply::execute(const QString &code)
{
    m_aborted = false;
    m_hasError = false;
    m_errorString = QString();

    m_reply = m_vk->call(code);
    connect(m_reply, SIGNAL(finished()), this, SLOT(reply_finished()));
    connect(this, SIGNAL(destroyed()), m_reply, SLOT(deleteLater()));
}

void CBaseReply::abort()
{
    if (m_reply)
    {
        m_aborted = true;
        m_reply->abort();
    }
}

void CBaseReply::reply_finished()
{
    bool isRetry = false; // true if we activete the timer

    if (m_reply->error() == QNetworkReply::NoError)
    {
        QVariantMap replyVariant = QtJson::parse(m_reply->readAll()).toMap();
        if (replyVariant.contains("error"))
        {
            int errorCode = replyVariant.value("error").toMap().value("error_code").toInt();

            // "too many request per second" -- sleep 0.5 second and try again
            if (errorCode == 6)
            {
                QTimer::singleShot(m_vk->retryApiInterval(), this, SLOT(start()));
                isRetry = true;
            }
            // the another error is fatal, no chance for correct them
            else
            {
                QString errorMsg = replyVariant.value("error").toMap().value("error_msg").toString();
                m_hasError = true;
                m_errorString = QString("%1: %2").arg(errorCode).arg(errorMsg);
            }
        }
        else if (replyVariant.contains("response"))
        {
            // the class heir should process the response
            processResponse(replyVariant.value("response"));
        }
    }
    // may be network problem?
    else if (m_reply->error() != QNetworkReply::OperationCanceledError)
    {
        QTimer::singleShot(m_vk->retryNetworkInterval(), this, SLOT(start()));
        isRetry = true;
    }

    m_reply->deleteLater();
    m_reply = 0;

    // if timer is active
    if (!isRetry)
    {
        emit finished();
    }
}
