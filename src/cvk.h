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
#ifndef CVK_H
#define CVK_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

class CVk : public QObject
{
    Q_OBJECT
public:
    explicit CVk(QObject *parent = 0);

    inline const QString &accessToken() const;
    inline int maximumDownloads() const;
    inline int appId() const;
    inline int retryApiInterval() const;
    inline int retryNetworkInterval() const;
    inline bool isAutoSync() const;
    inline bool isSyncOnStartup() const;
    inline int autoSyncInterval() const;

    void setAccessToken(const QString &accessToken);
    void setAppId(int appId);
    void setRetryApiInterval(int retryApiInterval);
    void setRetryNetworkInterval(int retryNetworkInterval);
    void setMaximumDownloads(int maximumDownloads);
    void setSyncOnStartup(bool enabled);
    void setAutoSync(bool enabled);
    void setAutoSyncInterval(int min);

    QNetworkReply *call(const QString &code);
    QNetworkAccessManager *dlNetwork() const;
    QNetworkAccessManager *apiNetwork() const;

signals:
    void accessTokenChanged(const QString &newAccessToken);
    void needSync();

private:
    QTimer *m_syncTimer;
    QString m_accessToken;
    QNetworkAccessManager *m_apiNetwork;
    QNetworkAccessManager *m_dlNetwork;
    int m_appId;
    int m_retryApiInterval;
    int m_retryNetworkInterval;
    int m_maximumDownloads;
    int m_autoSyncInterval;
    bool m_isAutoSync;
    bool m_isSyncOnStartup;

};

const QString &CVk::accessToken() const
{
    return m_accessToken;
}

int CVk::maximumDownloads() const
{
    return m_maximumDownloads;
}

int CVk::appId() const
{
    return m_appId;
}

int CVk::retryApiInterval() const
{
    return m_retryApiInterval;
}

int CVk::retryNetworkInterval() const
{
    return m_retryNetworkInterval;
}

bool CVk::isAutoSync() const
{
    return m_isAutoSync;
}

bool CVk::isSyncOnStartup() const
{
    return m_isSyncOnStartup;
}

int CVk::autoSyncInterval() const
{
    return m_autoSyncInterval;
}

#endif // CVK_H
