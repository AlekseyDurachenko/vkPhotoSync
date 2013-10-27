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
#ifndef CRELOCATEDIALOG_H
#define CRELOCATEDIALOG_H

#include <QDialog>

namespace Ui {
class RelocateDialog;
}

class CRelocateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CRelocateDialog(QWidget *parent = 0);
    ~CRelocateDialog();

    QString outputDirectory() const;
    bool isAutoRestart() const;
    void setOutputDirectory(const QString &outputDirectory) const;

protected:
    void accept();

private slots:
    void on_lineEdit_downloadDirectory_textChanged(const QString &arg1);
    void on_toolButton_downloadDirectory_clicked();

private:
    Ui::RelocateDialog *ui;

};

#endif // CRELOCATEDIALOG_H
