#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T15:14:56
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_TestVideoBufferTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
SOURCES += tst_TestVideoBufferTest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$PWD/../../include
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
