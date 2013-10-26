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
#include "cmainwindow.h"
#include "ui_mainwindow.h"
#include <QtXml>
#include <QMessageBox>
#include "cauthdialog.h"
#include "cstatuswidget.h"
#include "calbumitemmodel.h"
#include "cprogressdelegate.h"
#include "calbumadddialog.h"
#include "csettingsdialog.h"
#include "caboutdialog.h"
#include "global.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(G_FULL_PROGRAM_NAME());
    setWindowIcon(QIcon(":/icons/wicon.png"));

    //
    action_showHide = new QAction(this);
    connect(action_showHide, SIGNAL(triggered()), this, SLOT(slot_action_showHide_triggered()));

    //
    QMenu* systemTrayMenu = new QMenu(this);
    systemTrayMenu->addAction(action_showHide);
    systemTrayMenu->addSeparator();
    systemTrayMenu->addAction(ui->action_AddAlbum);
    systemTrayMenu->addSeparator();
    systemTrayMenu->addAction(ui->action_Login);
    systemTrayMenu->addAction(ui->action_Logout);
    systemTrayMenu->addSeparator();
    systemTrayMenu->addAction(ui->action_Start);
    systemTrayMenu->addAction(ui->action_Pause);
    systemTrayMenu->addSeparator();
    systemTrayMenu->addAction(ui->action_Quit);

    m_systemTrayIcon = new QSystemTrayIcon(QIcon(":/icons/wicon.png"), this);
    m_systemTrayIcon->setContextMenu(systemTrayMenu);
    m_systemTrayIcon->setToolTip(G_FULL_PROGRAM_NAME());
    connect(m_systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(slot_SystemTrayIcon_activated(QSystemTrayIcon::ActivationReason)));
    m_systemTrayIcon->show();

    //
    m_vk = new CVk(this);
    m_albumManager = new CAlbumItemManager(m_vk, this);
    m_albumItemModel = new CAlbumItemModel(m_albumManager, this);
    ui->album_treeView->setModel(m_albumItemModel);
    ui->album_treeView->setItemDelegateForColumn(4, new CProgressDelegate(this));
    connect(ui->album_treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateButtons()));
    connect(m_vk, SIGNAL(accessTokenChanged(QString)), this, SLOT(slot_vk_accessTokenChanged(QString)));
    connect(m_vk, SIGNAL(needSync()), this, SLOT(slot_vk_needSync()));

    CStatusWidget *statusWidget = new CStatusWidget(m_vk, this);
    ui->statusbar->addPermanentWidget(statusWidget);
    connect(statusWidget, SIGNAL(clicked()), this, SLOT(slot_status_clicked()));

    //
    loadSettings();
    loadQueue();
    updateButtons();
}

CMainWindow::~CMainWindow()
{
    saveSettings();
    saveQueue();
    delete ui;
}

void CMainWindow::slot_SystemTrayIcon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        setVisible(!isVisible());
    }
}

void CMainWindow::saveSettings()
{
    G_SETTINGS_INIT();

    // window && controll geometry
    settings.setValue("geometry", geometry());
    QList<QVariant> columnWidthList;
    for (int i = 0; i < m_albumItemModel->columnCount(); ++i)
    {
        columnWidthList.push_back(ui->album_treeView->columnWidth(i));
    }
    settings.setValue("album_column_width", columnWidthList);

    // configuration
    settings.setValue("access_token", m_vk->accessToken()); // store in wallet?
    settings.setValue("app_id", m_vk->appId());
    settings.setValue("maximum_downloads", m_vk->maximumDownloads());
    settings.setValue("retry_api_interval", m_vk->retryApiInterval());
    settings.setValue("retry_network_interval", m_vk->retryNetworkInterval());
    settings.setValue("autosync_interval", m_vk->autoSyncInterval());
    settings.setValue("is_autosync", m_vk->isAutoSync());
    settings.setValue("is_synconstartup", m_vk->isSyncOnStartup());
}

void CMainWindow::loadSettings()
{
    G_SETTINGS_INIT();

    // window && controll geometry    
    setGeometry(settings.value("geometry", geometry()).toRect());
    QList<QVariant> columnWidthList = settings.value("album_column_width", QList<QVariant>()).toList();
    for (int i = 0; i < m_albumItemModel->columnCount() && i < columnWidthList.count(); ++i)
    {
        ui->album_treeView->setColumnWidth(i, columnWidthList.value(i).toInt());
    }

    // configuration
    m_vk->setAccessToken(settings.value("access_token", "").toString());
    m_vk->setAppId(settings.value("app_id", m_vk->appId()).toInt());
    m_vk->setMaximumDownloads(settings.value("maximum_downloads", m_vk->maximumDownloads()).toInt());
    m_vk->setRetryApiInterval(settings.value("retry_api_interval", m_vk->retryApiInterval()).toInt());
    m_vk->setRetryNetworkInterval(settings.value("retry_network_interval", m_vk->retryNetworkInterval()).toInt());
    m_vk->setAutoSyncInterval(settings.value("autosync_interval", m_vk->autoSyncInterval()).toInt());
    m_vk->setAutoSync(settings.value("is_autosync", m_vk->isAutoSync()).toBool());
    m_vk->setSyncOnStartup(settings.value("is_synconstartup", m_vk->isSyncOnStartup()).toBool());
}

void CMainWindow::saveQueue()
{
    QDomDocument doc("ALBUMQUEUE");
    QDomElement albums = doc.createElement("albums");
    for (int i = 0; i < m_albumManager->count(); ++i)
    {
        QDomElement album = doc.createElement("album");
        album.setAttribute("owner_id", m_albumManager->value(i)->ownerId());
        album.setAttribute("album_id", m_albumManager->value(i)->albumId());
        album.setAttribute("owner_name", m_albumManager->value(i)->ownerName());
        album.setAttribute("album_name", m_albumManager->value(i)->albumName());
        album.setAttribute("datetime", m_albumManager->value(i)->dateTime().toString("yyyy.MM.dd HH:mm:ss.zzz"));
        album.setAttribute("photo_queue", m_albumManager->value(i)->photoQueue().join(","));
        album.setAttribute("count", m_albumManager->value(i)->count());
        album.setAttribute("state", m_albumManager->value(i)->state());
        album.setAttribute("error_string", m_albumManager->value(i)->errorString());
        album.setAttribute("output_directory", m_albumManager->value(i)->outputDirectory());
        albums.appendChild(album);
    }
    doc.appendChild(albums);

    QFile queueFile(queueFileName());
    if (queueFile.open(QIODevice::WriteOnly))
    {
        queueFile.write(doc.toByteArray());
    }
}

void CMainWindow::loadQueue()
{
    QFile queueFile(queueFileName());
    if (queueFile.open(QIODevice::ReadOnly))
    {
        QDomDocument doc("ALBUMQUEUE");
        if (doc.setContent(&queueFile))
        {
            QDomElement albums = doc.documentElement();
            QDomNode album = albums.firstChild();
            while (!album.isNull())
            {
                QDomElement albumElement = album.toElement();
                CAlbumItem *item  = new CAlbumItem;
                item->setOwnerId(albumElement.attribute("owner_id").toInt());
                item->setAlbumId(albumElement.attribute("album_id").toInt());
                item->setOwnerName(albumElement.attribute("owner_name"));
                item->setAlbumName(albumElement.attribute("album_name"));
                item->setDateTime(QDateTime::fromString(albumElement.attribute("datetime"), "yyyy.MM.dd HH:mm:ss.zzz"));
                if (!albumElement.attribute("photo_queue").isEmpty())
                {
                    item->setPhotoQueue(albumElement.attribute("photo_queue").split(","));
                }
                item->setCount(albumElement.attribute("count").toInt());
                item->setState(static_cast<CAlbumItem::State>(albumElement.attribute("state").toInt()));
                if (item->state() == CAlbumItem::Success && m_vk->isSyncOnStartup())
                {
                    item->setState(CAlbumItem::Updating);
                }
                item->setErrorString(albumElement.attribute("error_string"));
                item->setOutputDirectory(albumElement.attribute("output_directory"));
                m_albumManager->insert(item);

                album = album.nextSibling();
            }
        }
    }
}

QString CMainWindow::queueFileName() const
{
    G_SETTINGS_INIT();
    return QFileInfo(settings.fileName()).absolutePath() + QDir::separator() + "queue.xml";
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, tr("Quitting %1").arg(g_programName),
            tr("Are you sure you want to quit %1?").arg(g_programName),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        G_SETTINGS_INIT();
        settings.setValue("is_visible", isVisible());
        event->accept();
        qApp->quit();
    }
    else
    {
        event->ignore();
    }
}

void CMainWindow::hideEvent(QHideEvent *event)
{
    action_showHide->setText(tr("&Restore"));
    QMainWindow::hideEvent(event);
}

void CMainWindow::showEvent(QShowEvent *event)
{
    action_showHide->setText(tr("&Minimize"));
    QMainWindow::showEvent(event);
}

void CMainWindow::on_action_AddAlbum_triggered()
{
    CAlbumAddDialog addDialog(this);
    if (addDialog.exec() == QDialog::Accepted)
    {
        /* this code will not work. save for the future...
        bool duplicateFound = false;
        for (int index = 0; index < m_albumManager->count(); ++index)
        {
            if (m_albumManager->value(index)->ownerId() == addDialog.ownerId() &&
                    m_albumManager->value(index)->albumId() == addDialog.albumId() &&
                    m_albumManager->value(index)->outputDirectory() == addDialog.outputDirectory())
            {
                duplicateFound = true;
                break;
            }
        }
        */
        if (m_albumManager->contains(addDialog.ownerId(), addDialog.albumId()))
        {
            QMessageBox::information(this, tr("Information"), tr("The album already exists"));
        }
        else
        {
            CAlbumItem *item = new CAlbumItem;
            item->setOwnerId(addDialog.ownerId());
            item->setAlbumId(addDialog.albumId());
            item->setOwnerName(QString::number(addDialog.ownerId()));
            item->setAlbumName(QString::number(addDialog.albumId()));
            item->setOutputDirectory(addDialog.outputDirectory());
            if (addDialog.isAutoStart())
            {
                item->setState(CAlbumItem::Updating);
            }
            else
            {
                item->setState(CAlbumItem::Inactive);
            }
            m_albumManager->insert(item);
        }
    }
}

void CMainWindow::on_action_Login_triggered()
{
    CAuthDialog authDialog(m_vk->appId(), QvkAuthView::Photos|QvkAuthView::Friends|QvkAuthView::Groups, this);
    authDialog.resize(700, 430);
    if (authDialog.exec() == QDialog::Accepted)
    {
        m_vk->setAccessToken(authDialog.accessToken());
    }
}

void CMainWindow::on_action_Logout_triggered()
{
    if (QMessageBox::question(this, tr("Log out"), tr("Are you sure you want to log out?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        m_vk->setAccessToken(QString());
    }
}

void CMainWindow::on_action_Quit_triggered()
{
    close();
}

void CMainWindow::on_action_Start_triggered()
{
    foreach (QModelIndex index, ui->album_treeView->selectionModel()->selectedRows())
    {
        m_albumManager->start(index.row());
    }
}

void CMainWindow::on_action_Pause_triggered()
{
    foreach (QModelIndex index, ui->album_treeView->selectionModel()->selectedRows())
    {
        m_albumManager->pause(index.row());
    }
}

void CMainWindow::on_action_Delete_triggered()
{
    if (QMessageBox::question(this, tr("Deletion confirmation"),
            tr("Are you sure you want to delete the selected albums?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        int first = ui->album_treeView->selectionModel()->selectedRows().first().row();
        int last = ui->album_treeView->selectionModel()->selectedRows().last().row();
        for (int index = last; index >= first; --index)
        {
            m_albumManager->remove(index);
        }
    }
}

void CMainWindow::on_action_DecreasePriority_triggered()
{
    int first = ui->album_treeView->selectionModel()->selectedRows().first().row();
    int last = ui->album_treeView->selectionModel()->selectedRows().last().row();
    m_albumManager->decreasePriority(first, last);
}

void CMainWindow::on_action_IncreasePriority_triggered()
{
    int first = ui->album_treeView->selectionModel()->selectedRows().first().row();
    int last = ui->album_treeView->selectionModel()->selectedRows().last().row();
    m_albumManager->increasePriority(first, last);
}

void CMainWindow::on_action_Settings_triggered()
{
    CSettingsDialog settingsDialog(this);
    settingsDialog.setMaximumDownloads(m_vk->maximumDownloads());
    settingsDialog.setRetryApiInterval(m_vk->retryApiInterval());
    settingsDialog.setRetryNetworkInterval(m_vk->retryNetworkInterval());
    settingsDialog.setAppId(m_vk->appId());
    settingsDialog.setAutoSync(m_vk->isAutoSync());
    settingsDialog.setSyncOnStarttup(m_vk->isSyncOnStartup());
    settingsDialog.setAutoSyncInterval(m_vk->autoSyncInterval());
    if (settingsDialog.exec() == QDialog::Accepted)
    {
        m_vk->setMaximumDownloads(settingsDialog.maximumDownloads());
        m_vk->setRetryApiInterval(settingsDialog.retryApiInterval());
        m_vk->setRetryNetworkInterval(settingsDialog.retryNetworkInterval());
        m_vk->setAppId(settingsDialog.appId());
        m_vk->setAutoSyncInterval(settingsDialog.autoSyncInterval());
        m_vk->setAutoSync(settingsDialog.isAutoSync());
        m_vk->setSyncOnStartup(settingsDialog.isSyncOnStartup());
    }
}

void CMainWindow::on_action_About_triggered()
{
    CAboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void CMainWindow::on_action_AboutQt_triggered()
{
    qApp->aboutQt();
}

void CMainWindow::on_album_treeView_customContextMenuRequested(const QPoint &)
{
    QMenu menu(this);
    menu.addAction(ui->action_AddAlbum);
    menu.addSeparator();
    menu.addAction(ui->action_Start);
    menu.addAction(ui->action_Pause);
    menu.addAction(ui->action_Delete);
    menu.exec(QCursor::pos());
}

void CMainWindow::slot_status_clicked()
{
    if (m_vk->accessToken().isEmpty())
    {
        on_action_Login_triggered();
    }
    else
    {
        on_action_Logout_triggered();
    }
}

void CMainWindow::slot_vk_accessTokenChanged(const QString &accessToken)
{
    ui->action_Logout->setDisabled(accessToken.isEmpty());
}

void CMainWindow::slot_vk_needSync()
{
    for (int index = 0; index < m_albumManager->count(); ++index)
    {
        if (m_albumManager->value(index)->state() == CAlbumItem::Success)
        {
            m_albumManager->start(index);
        }
    }
}

void CMainWindow::slot_action_showHide_triggered()
{
    setVisible(!isVisible());
}

void CMainWindow::updateButtons()
{
    ui->action_Start->setEnabled(ui->album_treeView->selectionModel()->selectedRows().count());
    ui->action_Pause->setEnabled(ui->album_treeView->selectionModel()->selectedRows().count());
    ui->action_Delete->setEnabled(ui->album_treeView->selectionModel()->selectedRows().count());
    ui->action_IncreasePriority->setEnabled(ui->album_treeView->selectionModel()->selectedRows().count());
    ui->action_DecreasePriority->setEnabled(ui->album_treeView->selectionModel()->selectedRows().count());
}
