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
#ifndef CFILEDOWNLOADER_H
#define CFILEDOWNLOADER_H

#include <QObject>
#include <QUrl>
class CVk;
class QNetworkReply;
class QFile;

class CFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit CFileDownloader(CVk *vk, QObject *parent = 0);
    ~CFileDownloader();

    inline const QUrl &url() const;
    inline const QString &fileName() const;
    void setUrl(const QUrl &url);
    void setFileName(const QString &fileName);

    inline bool hasError() const;
    inline const QString &errorString() const;
    inline bool isAborted() const;

public slots:
    void start();
    void abort();

signals:
    void finished(); // please, use for this signal the Qt:QueueConnection

private slots:
    void slot_readyRead();
    void slot_finished();

private:
    QString tmpFileName(const QString &fileName);

private:
    QUrl m_url;
    QString m_fileName;

    CVk *m_vk;
    QNetworkReply *m_reply;
    QFile *m_file;

    bool m_isAborted;
    bool m_hasError;
    QString m_errorString;

};

const QUrl &CFileDownloader::url() const
{
    return m_url;
}

const QString &CFileDownloader::fileName() const
{
    return m_fileName;
}

bool CFileDownloader::hasError() const
{
    return m_hasError;
}

const QString &CFileDownloader::errorString() const
{
    return m_errorString;
}

bool CFileDownloader::isAborted() const
{
    return m_isAborted;
}

#endif // CFILEDOWNLOADER_H
