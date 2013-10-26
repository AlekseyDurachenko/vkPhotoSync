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
#ifndef CBASEREPLY_H
#define CBASEREPLY_H

#include <QObject>
#include <QVariantMap>
class QNetworkReply;
class CVk;

class CBaseReply : public QObject
{
    Q_OBJECT
public:
    explicit CBaseReply(CVk *vk, QObject *parent = 0);

    inline bool hasError() const;
    inline const QString &errorString() const;
    inline bool isAborted() const;

public slots:
    virtual void start() = 0;
    void abort();

protected:
    void execute(const QString &code);
    virtual void processResponse(const QVariant &response) = 0;

signals:
    void finished();

private slots:
    void reply_finished();

private:
    bool m_aborted;
    bool m_hasError;
    QString m_errorString;

    CVk *m_vk;
    QNetworkReply *m_reply;

};

bool CBaseReply::hasError() const
{
    return m_hasError;
}

const QString &CBaseReply::errorString() const
{
    return m_errorString;
}

bool CBaseReply::isAborted() const
{
    return m_aborted;
}

#endif // CBASEREPLY_H
