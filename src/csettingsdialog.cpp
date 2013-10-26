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
#include "csettingsdialog.h"
#include "ui_settingsdialog.h"

CSettingsDialog::CSettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

CSettingsDialog::~CSettingsDialog()
{
    delete ui;
}

int CSettingsDialog::maximumDownloads() const
{
    return ui->spinBox_maximumDownloads->value();
}

int CSettingsDialog::appId() const
{
    return ui->spinBox_appID->value();
}

int CSettingsDialog::retryApiInterval() const
{
    return ui->spinBox_retryApiInterval->value();
}

int CSettingsDialog::retryNetworkInterval() const
{
    return ui->spinBox_retryNetworkInterval->value();
}

bool CSettingsDialog::isSyncOnStartup() const
{
    return ui->checkBox_syncOnStartup->isChecked();
}

bool CSettingsDialog::isAutoSync() const
{
    return ui->checkBox_enableAutoSync->isChecked();
}

int CSettingsDialog::autoSyncInterval() const
{
    return ui->spinBox_autoSyncInterval->value();
}

void CSettingsDialog::setMaximumDownloads(int maximumDownloads)
{
    ui->spinBox_maximumDownloads->setValue(maximumDownloads);
}

void CSettingsDialog::setAppId(int appId)
{
    ui->spinBox_appID->setValue(appId);
}

void CSettingsDialog::setRetryApiInterval(int retryApiInterval)
{
    ui->spinBox_retryApiInterval->setValue(retryApiInterval);
}

void CSettingsDialog::setRetryNetworkInterval(int retryNetworkInterval)
{
    ui->spinBox_retryNetworkInterval->setValue(retryNetworkInterval);
}

void CSettingsDialog::setSyncOnStarttup(bool enabled)
{
    ui->checkBox_syncOnStartup->setChecked(enabled);
}

void CSettingsDialog::setAutoSync(bool enabled)
{
    ui->checkBox_enableAutoSync->setChecked(enabled);
}

void CSettingsDialog::setAutoSyncInterval(int min)
{
    ui->spinBox_autoSyncInterval->setValue(min);
}
