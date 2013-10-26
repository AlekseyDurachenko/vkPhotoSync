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
#include "cvk.h"
#include "consts.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>
#include <QUrl>

CVk::CVk(QObject *parent)
    : QObject(parent)
{
    m_syncTimer = new QTimer(this);
    m_apiNetwork = new QNetworkAccessManager(this);
    m_dlNetwork = new QNetworkAccessManager(this);
    m_accessToken = QString();
    m_appId = g_defaultAppId;
    m_retryApiInterval = g_defaultRetryApiInterval;
    m_retryNetworkInterval = g_defaultRetryNetworkInterval;
    m_maximumDownloads = g_defaultMaximumDownloads;
    m_autoSyncInterval = g_defaultAutoSyncInterval;
    m_isAutoSync = g_defaultIsAutoSync;
    m_isSyncOnStartup = g_defaultIsSyncOnStartup;
    connect(m_syncTimer, SIGNAL(timeout()), this, SIGNAL(needSync()));
}

void CVk::setAccessToken(const QString &accessToken)
{
    m_accessToken = accessToken;
    emit accessTokenChanged(m_accessToken);
}

void CVk::setAppId(int appId)
{
    if (appId != m_appId)
    {
        m_appId = appId;
        setAccessToken(QString());
    }
}

void CVk::setRetryApiInterval(int retryApiInterval)
{
    m_retryApiInterval = retryApiInterval;
}

void CVk::setRetryNetworkInterval(int retryNetworkInterval)
{
    m_retryNetworkInterval = retryNetworkInterval;
}

void CVk::setMaximumDownloads(int maximumDownloads)
{
    if (m_maximumDownloads != maximumDownloads)
    {
        m_maximumDownloads = maximumDownloads;
    }
}

void CVk::setSyncOnStartup(bool enabled)
{
    m_isSyncOnStartup = enabled;
}

void CVk::setAutoSync(bool enabled)
{
    m_isAutoSync = enabled;
    if (m_isAutoSync && !m_syncTimer->isActive())
    {
        m_syncTimer->start(m_autoSyncInterval * 60000);
    }
    else if (!m_isAutoSync && m_syncTimer->isActive())
    {
        m_syncTimer->stop();
    }
}

void CVk::setAutoSyncInterval(int min)
{
    m_autoSyncInterval = min;
    m_syncTimer->setInterval(m_autoSyncInterval * 60000);
}

QNetworkReply *CVk::call(const QString &code)
{
    QUrl apiUrl = QUrl(QString("https://api.vk.com/method/execute"));

    apiUrl.addQueryItem("v", g_protocolVersion);
    if (!m_accessToken.isEmpty())
        apiUrl.addQueryItem("access_token", m_accessToken);
    apiUrl.addEncodedQueryItem("code", QUrl::toPercentEncoding(code));

    QNetworkRequest request;
    request.setUrl(apiUrl);
    request.setRawHeader("User-Agent", g_userAgent);
    return m_apiNetwork->get(request);
}


QNetworkAccessManager *CVk::dlNetwork() const
{
    return m_dlNetwork;
}

QNetworkAccessManager *CVk::apiNetwork() const
{
    return m_apiNetwork;
}
