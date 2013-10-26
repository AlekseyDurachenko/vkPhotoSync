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
#include "cauthdialog.h"
#include "ui_authdialog.h"
#include <QMessageBox>

CAuthDialog::CAuthDialog(int appId, QvkAuthView::Permissions permissions, QWidget *parent)
    : QDialog(parent), ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    ui->authWidget->setAppId(appId);
    ui->authWidget->setRequiredPermissions(permissions);
    ui->authWidget->openAuthPage();
    connect(ui->authWidget, SIGNAL(authFail(QString,QString)),
            this, SLOT(authFailed(QString,QString)));
    connect(ui->authWidget, SIGNAL(authSuccess(QString,int,int)),
            this, SLOT(authSuccessed(QString,int,int)));
}

CAuthDialog::~CAuthDialog()
{
    delete ui;
}

void CAuthDialog::authFailed(const QString &errorCode, const QString &errorString)
{
    QMessageBox::warning(this, tr("Authentication failed"), QString("Code %1: %2").arg(errorCode, errorString));
    ui->authWidget->openAuthPage();
}

void CAuthDialog::authSuccessed(const QString &accessToken, int expiresInSecs, int userId)
{
    m_accessToken = accessToken;
    m_expiresInSecs = expiresInSecs;
    m_userId = userId;
    accept();
}
