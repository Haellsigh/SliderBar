#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T21:10:12
#
#-------------------------------------------------

QT       += widgets serialport

TARGET = SlideBar
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    serialprotocol.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    serialprotocol.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
