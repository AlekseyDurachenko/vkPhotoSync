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
#include "qvkauthview.h"

QvkAuthView::QvkAuthView(QWidget *parent)
    : QWebView(parent)
{
    m_appId = 1;
    m_requiredPermissions = None;
    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(on_urlChanged(QUrl)));
}

void QvkAuthView::setAppId(int appId)
{
    m_appId = appId;
}

void QvkAuthView::setRequiredPermissions(Permissions requirePermissions)
{
    m_requiredPermissions = requirePermissions;
}

void QvkAuthView::openAuthPage()
{
    QUrl url("http://api.vkontakte.ru/oauth/authorize");
    url.addQueryItem("client_id", QString::number(m_appId));
    url.addQueryItem("scope", QString::number(m_requiredPermissions));
    url.addQueryItem("display", "mobile");
    url.addQueryItem("v", "5.2");
    url.addQueryItem("redirect_uri", "http://api.vkontakte.ru/blank.html");
    url.addQueryItem("response_type", "token");
    page()->networkAccessManager()->setCookieJar(new QNetworkCookieJar()); // reset all cookie
    load(url);
}

void QvkAuthView::on_urlChanged(const QUrl &newUrl)
{
    QUrl url = newUrl.toString().replace("#","?");

    if (url.hasQueryItem("error"))
    {
        emit authFail(url.queryItemValue("error"), url.queryItemValue("error_description"));
        return;
    }

    if (url.hasQueryItem("access_token"))
    {
        emit authSuccess(url.queryItemValue("access_token"), url.queryItemValue("expires_in").toInt(),
                url.queryItemValue("user_id").toInt());
        return;
    }
}
