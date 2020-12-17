#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T14:57:00
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_TestAttributeTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11
SOURCES += tst_TestAttributeTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


INCLUDEPATH += $$PWD/../../include
INCLUDEPATH += $$PWD/../../dependencies/include

LIBS += -L$$PWD/../../lib

CONFIG(debug, debug|release) {
        LIBS += -lsmf-d
        DESTDIR = $$PWD/../../bin/debug
    }   else {
        LIBS += -lsmf
        DESTDIR = $$PWD/../../bin/release
    }

unix {
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/log4cplus -llog4cplus
}
