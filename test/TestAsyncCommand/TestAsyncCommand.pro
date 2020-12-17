#-------------------------------------------------
#
# Project created by QtCreator 2015-03-26T13:29:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TestAsyncCommand
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += $$PWD/../../include
LIBS += -L$$PWD/../../lib

win32 {

CONFIG += debug_and_release

CONFIG(debug, debug|release) {
        LIBS += -lsmf-d
    }   else {
        LIBS += -lsmf
    }

}
