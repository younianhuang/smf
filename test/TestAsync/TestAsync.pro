#-------------------------------------------------
#
# Project created by QtCreator 2015-03-27T17:02:16
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_TestAsyncTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11
SOURCES += tst_TestAsyncTest.cpp
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

win32 {

}

unix {
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/log4cplus -llog4cplus
}
