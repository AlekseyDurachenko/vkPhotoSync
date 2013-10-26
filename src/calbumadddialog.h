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
#ifndef CALBUMADDDIALOG_H
#define CALBUMADDDIALOG_H

#include <QDialog>
#include <QRegExp>

namespace Ui {
class AlbumAddDialog;
}

class CAlbumAddDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CAlbumAddDialog(QWidget *parent = 0);
    ~CAlbumAddDialog();

    QString outputDirectory() const;
    int ownerId() const;
    int albumId() const;
    bool isAutoStart() const;

private slots:
    void on_downloadDirectory_toolButton_clicked();
    void on_link_lineEdit_textChanged(const QString &text);
    void slot_id_changed();
    void accept();

private:
    Ui::AlbumAddDialog *ui;
    QRegExp m_rxAlbum;

};

#endif // CALBUMADDDIALOG_H
