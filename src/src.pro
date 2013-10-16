#-------------------------------------------------
#
# Project created by QtCreator 2013-09-30T10:46:13
#
#-------------------------------------------------

QT       += core gui declarative
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CorrelationAnalysis
DESTDIR = $$PWD/../../installer
TEMPLATE = app

win32{
    RC_FILE = res/icon.rc
}


include($$PWD/QwtItems.pri)
include($$PWD/File.pri)
include($$PWD/Setter.pri)
include($$PWD/Algorithm.pri)

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

RESOURCES += \
    res.qrc
