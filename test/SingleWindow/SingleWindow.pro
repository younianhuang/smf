#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T16:40:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SingleWindow
TEMPLATE = app


INCLUDEPATH += $$PWD/../../include
INCLUDEPATH += $$PWD/../../include/renderer/VideoRenderer
DEPENDPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include/renderer/VideoRenderer
LIBS += -L$$PWD/../../lib


SOURCES += main.cpp\
        MainWindow.cpp \
    RenderingWindow.cpp

HEADERS  += MainWindow.h \
    RenderingWindow.h

FORMS    += MainWindow.ui \
    RenderingWindow.ui

win32 {


CONFIG += debug_and_release

CONFIG(debug, debug|release) {
        LIBS += -lsmf-d -lglvr-d
    }   else {
        LIBS += -lsmf -lglvr
    }

}
