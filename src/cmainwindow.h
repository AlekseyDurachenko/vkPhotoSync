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
#ifndef QVKMAINWINDOW_H
#define QVKMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
class CAlbumItemManager;
class CAlbumItemModel;
class CVk;

namespace Ui {
class MainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();
    
private slots:
    void on_action_AddAlbum_triggered();
    void on_action_Login_triggered();
    void on_action_Logout_triggered();
    void on_action_Quit_triggered();
    void on_action_Start_triggered();
    void on_action_Pause_triggered();
    void on_action_Delete_triggered();
    void on_action_DecreasePriority_triggered();
    void on_action_IncreasePriority_triggered();
    void on_action_Settings_triggered();
    void on_action_About_triggered();
    void on_action_AboutQt_triggered();
    void on_album_treeView_customContextMenuRequested(const QPoint &);

    void slot_status_clicked();
    void slot_vk_accessTokenChanged(const QString &accessToken);
    void slot_vk_needSync();
    void slot_action_showHide_triggered();
    void slot_SystemTrayIcon_activated(QSystemTrayIcon::ActivationReason);
    void updateButtons();

private:
    void saveSettings();
    void loadSettings();
    void saveQueue();
    void loadQueue();
    QString queueFileName() const;

protected:
    void closeEvent(QCloseEvent* event);
    void hideEvent(QHideEvent * event);
    void showEvent(QShowEvent * event);

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon* m_systemTrayIcon;
    QAction *action_showHide;
    CAlbumItemManager *m_albumManager;
    CAlbumItemModel *m_albumItemModel;
    CVk *m_vk;

};

#endif // QVKMAINWINDOW_H
