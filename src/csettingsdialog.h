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
#ifndef CSETTINGSDIALOG_H
#define CSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class CSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CSettingsDialog(QWidget *parent = 0);
    ~CSettingsDialog();

    int maximumDownloads() const;
    int appId() const;
    int retryApiInterval() const;
    int retryNetworkInterval() const;
    bool isSyncOnStartup() const;
    bool isAutoSync() const;
    int autoSyncInterval() const;
    void setMaximumDownloads(int maximumDownloads);
    void setAppId(int appId);
    void setRetryApiInterval(int retryApiInterval);
    void setRetryNetworkInterval(int retryNetworkInterval);
    void setSyncOnStarttup(bool enabled);
    void setAutoSync(bool enabled);
    void setAutoSyncInterval(int min);

private:
    Ui::SettingsDialog *ui;

};

#endif // CSETTINGSDIALOG_H
