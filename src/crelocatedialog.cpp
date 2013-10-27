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
#include "src/crelocatedialog.h"
#include "ui_relocatedialog.h"
#include "global.h"
#include <QFileDialog>
#include <QSettings>
#include <QDir>

CRelocateDialog::CRelocateDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::RelocateDialog)
{
    ui->setupUi(this);
    ui->buttonBox->buttons().value(0)->setEnabled(false);

    G_SETTINGS_INIT();
    ui->checkBox_restart->setChecked(settings.value("crealocatedialog/autorestart", false).toBool());
}

CRelocateDialog::~CRelocateDialog()
{
    delete ui;
}

QString CRelocateDialog::outputDirectory() const
{
    return ui->lineEdit_downloadDirectory->text();
}

void CRelocateDialog::setOutputDirectory(const QString &outputDirectory) const
{
    ui->lineEdit_downloadDirectory->setText(outputDirectory);
}

bool CRelocateDialog::isAutoRestart() const
{
    return ui->checkBox_restart->isChecked();
}

void CRelocateDialog::accept()
{
    G_SETTINGS_INIT();
    settings.setValue("last_download_directory", ui->lineEdit_downloadDirectory->text());
    settings.setValue("crealocatedialog/autorestart", isAutoRestart());

    QDialog::accept();
}

void CRelocateDialog::on_lineEdit_downloadDirectory_textChanged(const QString &text)
{
    ui->buttonBox->buttons().value(0)->setDisabled(text.isEmpty());
}

void CRelocateDialog::on_toolButton_downloadDirectory_clicked()
{
    QString lastDirectory = ui->lineEdit_downloadDirectory->text();
    if (lastDirectory.isEmpty())
    {
        G_SETTINGS_INIT();
        lastDirectory = settings.value("last_download_directory", QDir::homePath()).toString();
    }

    QString dirname = QFileDialog::getExistingDirectory(this, tr("Select the download directory"),
            lastDirectory, QFileDialog::ShowDirsOnly);

    if (!dirname.isEmpty())
    {
        ui->lineEdit_downloadDirectory->setText(dirname);
    }
}
