# Copyright 2013, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# project options
TARGET          = vkphotosync
TEMPLATE        = app
VERSION         = 0.1
DESTDIR         = bin

# build options
CONFIG         += console debug_and_release
QT             += core gui webkit network xml

# build direcotries
OBJECTS_DIR     = build/release_obj
MOC_DIR         = build/release_moc
UI_DIR          = build/release_ui
RCC_DIR         = build/release_rcc

# defines
DEFINES        +=                                           \

# 3rd include path
INCLUDEPATH    +=                                           \

# sources
INCLUDEPATH    +=                                           \
    libs/qt-json                                            \
    src                                                     \

HEADERS        +=                                           \
    libs/qt-json/json.h                                     \
    src/consts.h                                            \
    src/global.h                                            \
    src/version.h                                           \
    src/qvkauthview.h                                       \
    src/cprogressdelegate.h                                 \
    src/cstatuswidget.h                                     \
    src/calbumreply.h                                       \
    src/cvk.h                                               \
    src/cmainwindow.h                                       \
    src/cfiledownloader.h                                   \
    src/ciamreply.h                                         \
    src/cauthdialog.h                                       \
    src/calbumadddialog.h                                   \
    src/calbumitem.h                                        \
    src/calbumitemmanager.h                                 \
    src/calbumitemmodel.h                                   \
    src/cbasereply.h                                        \
    src/csettingsdialog.h                                   \
    src/caboutdialog.h                                      \
    src/crelocatedialog.h                                   \

SOURCES        +=                                           \
    libs/qt-json/json.cpp                                   \
    src/qvkauthview.cpp                                     \
    src/cprogressdelegate.cpp                               \
    src/cstatuswidget.cpp                                   \
    src/calbumreply.cpp                                     \
    src/cvk.cpp                                             \
    src/cmainwindow.cpp                                     \
    src/cfiledownloader.cpp                                 \
    src/ciamreply.cpp                                       \
    src/cauthdialog.cpp                                     \
    src/calbumadddialog.cpp                                 \
    src/calbumitem.cpp                                      \
    src/calbumitemmanager.cpp                               \
    src/calbumitemmodel.cpp                                 \
    src/cbasereply.cpp                                      \
    src/csettingsdialog.cpp                                 \
    src/caboutdialog.cpp                                    \
    src/crelocatedialog.cpp                                 \


FORMS          +=                                           \
    ui/mainwindow.ui                                        \
    ui/albumadddialog.ui                                    \
    ui/authdialog.ui                                        \
    ui/settingsdialog.ui                                    \
    ui/aboutdialog.ui                                       \
    ui/relocatedialog.ui                                    \

RESOURCES      +=                                           \
    qrc/about.qrc                                           \
    qrc/icons.qrc                                           \
    qrc/vkscript.qrc                                        \

OTHER_FILES    +=                                           \
    AUTHORS                                                 \
    CHANGELOG                                               \
    LICENSE                                                 \
    README                                                  \

# if you want to test the project, do: qmake "QT += testlib"
contains(QT, testlib) {
    SOURCES   +=                                            \

    HEADERS   +=                                            \

} else {
    SOURCES   +=                                            \
        src/main.cpp                                        \

}

# debug
build_pass:CONFIG(debug, debug|release) {
    TARGET      = $$join(TARGET,,,d)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
    MOC_DIR     = $$join(MOC_DIR,,,d)
    UI_DIR      = $$join(UI_DIR,,,d)
    RCC_DIR     = $$join(RCC_DIR,,,d)

    win32 {
    }

    unix {
        LIBS +=
    }
}

# release
build_pass:CONFIG(release, debug|release) {
    win32 {
    }

    unix {
        LIBS +=
    }
}
