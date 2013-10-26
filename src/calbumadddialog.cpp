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
#include "calbumadddialog.h"
#include "ui_albumadddialog.h"
#include "global.h"
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QRegExp>
#include <QApplication>
#include <QClipboard>

CAlbumAddDialog::CAlbumAddDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AlbumAddDialog)
{
    ui->setupUi(this);
    ui->buttonBox->buttons().value(0)->setEnabled(false);
    connect(ui->ownerId_spinBox, SIGNAL(valueChanged(int)), SLOT(slot_id_changed()));
    connect(ui->albumId_spinBox, SIGNAL(valueChanged(int)), SLOT(slot_id_changed()));
    m_rxAlbum.setPattern("^(http://){0,1}vk.com/.*album([-]{0,1}[0-9]{1,})_([0-9]{1,})$");

    // try to paste link from clipboard
    if (m_rxAlbum.exactMatch(QApplication::clipboard()->text()))
    {
        ui->link_lineEdit->setText(QApplication::clipboard()->text());
    }

    G_SETTINGS_INIT();
    ui->downloadDirectory_lineEdit->setText(settings.value("last_download_directory", QDir::homePath()).toString());
    ui->checkBox_autoStart->setChecked(settings.value("calbumdialog/autostart", true).toBool());
}

CAlbumAddDialog::~CAlbumAddDialog()
{
    delete ui;
}

QString CAlbumAddDialog::outputDirectory() const
{
    return ui->downloadDirectory_lineEdit->text();
}

int CAlbumAddDialog::ownerId() const
{
    return ui->ownerId_spinBox->value();
}

int CAlbumAddDialog::albumId() const
{
    return ui->albumId_spinBox->value();
}

bool CAlbumAddDialog::isAutoStart() const
{
    return ui->checkBox_autoStart->isChecked();
}

void CAlbumAddDialog::on_downloadDirectory_toolButton_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this,
        tr("Select the download directory"), ui->downloadDirectory_lineEdit->text(), QFileDialog::ShowDirsOnly);

    if (!dirname.isEmpty())
    {        
        ui->downloadDirectory_lineEdit->setText(dirname);
    }
}

void CAlbumAddDialog::on_link_lineEdit_textChanged(const QString &text)
{
    if (m_rxAlbum.exactMatch(text))
    {
        ui->ownerId_spinBox->setValue(m_rxAlbum.capturedTexts().value(2).toInt());
        ui->albumId_spinBox->setValue(m_rxAlbum.capturedTexts().value(3).toInt());
    }
    else
    {
        ui->ownerId_spinBox->setValue(0);
        ui->albumId_spinBox->setValue(0);
    }
}

void CAlbumAddDialog::slot_id_changed()
{
    ui->buttonBox->buttons().value(0)->setEnabled(
            ui->albumId_spinBox->value() && ui->albumId_spinBox->value());
}

void CAlbumAddDialog::accept()
{
    G_SETTINGS_INIT();
    settings.setValue("last_download_directory", ui->downloadDirectory_lineEdit->text());
    settings.setValue("calbumdialog/autostart", isAutoStart());

    QDialog::accept();
}
