#-------------------------------------------------
#
# Project created by QtCreator 2015-03-30T14:02:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RenderingVideo
TEMPLATE = app

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
SOURCES += main.cpp\
    ../common/CPUUsage.cpp \
    ../common/MemoryUsage.cpp \
    ../common/ViewPane.cpp \
    ../common/Screenshot.cpp \
    ../common/GLMarkLog.cpp \
    ../common/RenderingVideoWindow.cpp \
    ../common/RenderingWindow.cpp

HEADERS  += \
    ../common/CommonConfig.h \
    ../common/CPUUsage.h \
    ../common/MemoryUsage.h \
    ../common/GLVRWidget.h \
    ../common/ViewPane.h \
    ../common/Screenshot.h \
    ../common/GLMarkLog.h \
    ../common/RenderingVideoWindow.h \
    ../common/RenderingWindow.h

FORMS    += \
    ../common/RenderingVideoWindow.ui

INCLUDEPATH += $$PWD/../../dependencies/include
INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
INCLUDEPATH += $$PWD/../../include/renderer/VideoRenderer
DEPENDPATH += $$PWD/../../include/renderer/VideoRenderer
INCLUDEPATH += $$PWD/../../sample/common
DEPENDPATH += $$PWD/../../sample/common
LIBS += -L$$PWD/../../lib
LIBS += -L$$PWD/../../lib/

CONFIG(debug, debug|release) {
        LIBS += -lsmf-d -lglvr-d
        DESTDIR = $$PWD/../../bin/debug
        DEFINES += SMF_DISABLE_TRACE_LOG
    }   else {
        LIBS += -lsmf -lglvr
        DESTDIR = $$PWD/../../bin/release
        DEFINES += SMF_DISABLE_DEBUG_LOG SMF_DISABLE_TRACE_LOG
}

win32 {
    LIBS += -lPdh
}

unix {
    QT       +=  x11extras

    HEADERS += XVideoRenderer.h
    SOURCES += XVideoRenderer.cpp

    LIBS += -L$$PWD/../../dependencies/lib/gcc64/glew -lGLEW
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/log4cplus -llog4cplus
    LIBS += -lXv -lX11
}
