#-------------------------------------------------
#
# Project created by QtCreator 2013-10-08T16:13:44
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = FileTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += FileTest.cpp \
    ../../src/File.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../src/File.h
