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
#ifndef GLOBAL_H
#define GLOBAL_H

#include "version.h"

#define G_SETTINGS_INIT()                                               \
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,      \
        QString(g_projectFamily) + QDir::separator() + QString(g_projectName), g_programName);

#define G_FULL_PROGRAM_NAME()                                           \
    (QString("%1 - %2.%3.%4%5")                                         \
        .arg(g_programName).arg(g_versionMajor).arg(g_versionMinor)     \
        .arg(g_versionBuild).arg(g_versionStatusShort))                 \

#define G_SHORT_PROGRAM_NAME()                                          \
    (QString("%1").arg(g_programName))                                  \

#define G_PROGRAM_ABOUT()                                               \
    ("<center>" + G_FULL_PROGRAM_NAME() + "<br>" +                      \
    QString("Author: %1, %2").arg(g_authors).arg(g_years) +             \
    "</center>")                                                        \

#endif
