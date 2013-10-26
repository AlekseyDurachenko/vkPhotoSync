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
#ifndef QVKAUTHVIEW_H
#define QVKAUTHVIEW_H

#include <QtWebKit>
#include <QtNetwork>

class QvkAuthView : public QWebView
{
    Q_OBJECT
public:
    enum Permission // more details about vk permission -- http://vk.com/dev/permissions
    {
        None = 0,
        Notify = 1,
        Friends = 2,
        Photos = 4,
        Audio = 8,
        Video = 16,
        Docs = 131072,
        Notes = 2048,
        Pages = 128,
        AttacheToMenu = 256,
        Status = 1024,
        Offers = 32,
        Questions = 64,
        Wall = 8192,
        Groups = 262144,
        Messages = 4096,
        Notifications = 524288,
        Stats = 1048576,
        Ads = 32768,
        Offline = 65536
    };
    Q_DECLARE_FLAGS(Permissions, Permission)

    explicit QvkAuthView(QWidget *parent = 0);
    inline int appId() const;
    inline Permissions requiredPermissions() const;
    void setAppId(int appId);
    void setRequiredPermissions(Permissions requiredPermissions);
    void openAuthPage();

signals:
    void authSuccess(const QString &accessToken, int expiresInSecs, int userId);
    void authFail(const QString &errorCode, const QString &errorDesc);

private slots:
    void on_urlChanged(const QUrl &newUrl);

private:
    int m_appId;
    Permissions m_requiredPermissions;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(QvkAuthView::Permissions)

inline int QvkAuthView::appId() const
{
    return m_appId;
}

inline QvkAuthView::Permissions QvkAuthView::requiredPermissions() const
{
    return m_requiredPermissions;
}

#endif // QVKAUTHVIEW_H
