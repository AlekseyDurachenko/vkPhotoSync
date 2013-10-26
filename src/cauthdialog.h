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
#ifndef CAUTHDIALOG_H
#define CAUTHDIALOG_H

#include <QDialog>
#include "qvkauthview.h"

namespace Ui {
class AuthDialog;
}

class CAuthDialog : public QDialog
{
    Q_OBJECT
    
public:
    CAuthDialog(int appId, QvkAuthView::Permissions permissions, QWidget *parent = 0);
    ~CAuthDialog();
    inline int expiresInSecs() const;
    inline int userId() const;
    inline const QString &accessToken() const;

private slots:
    void authFailed(const QString &errorCode, const QString &errorString);
    void authSuccessed(const QString &accessToken, int expiresInSecs, int userId);

private:
    Ui::AuthDialog *ui;
    QString m_accessToken;
    int m_expiresInSecs;
    int m_userId;
};

int CAuthDialog::expiresInSecs() const
{
    return m_expiresInSecs;
}

int CAuthDialog::userId() const
{
    return m_userId;
}

const QString &CAuthDialog::accessToken() const
{
    return m_accessToken;
}

#endif // CAUTHDIALOG_H
