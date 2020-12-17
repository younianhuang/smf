#-------------------------------------------------
#
# Project created by QtCreator 2015-06-30T16:34:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleMediaPlayer
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    ../common/CPUUsage.cpp \
    ../common/MemoryUsage.cpp \
    ../common/ViewPane.cpp \
    ../common/Screenshot.cpp \
    ../common/GLMarkLog.cpp \
    ../common/SimpleMediaPlayerWindow.cpp \
    ../common/ViewTile.cpp

HEADERS  += \
    ../common/CommonConfig.h \
    ../common/CPUUsage.h \
    ../common/MemoryUsage.h \
    ../common/ViewPane.h \
    ../common/Screenshot.h \
    ../common/GLMarkLog.h \
    ../common/SimpleMediaPlayerWindow.h \
    ../common/ViewTile.h

FORMS    += \
    ../common/SimpleMediaPlayerWindow.ui

LIBS += -L$$PWD/../../lib
DEPENDPATH += -L$$PWD/../../lib

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
INCLUDEPATH += $$PWD/../../include/renderer/VideoRenderer
DEPENDPATH += $$PWD/../../include/renderer/VideoRenderer
INCLUDEPATH += $$PWD/../../sample/common
DEPENDPATH += $$PWD/../../sample/common

CONFIG(debug, debug|release) {
        LIBS += -lsmf-d -lglvr-d
        DESTDIR = $$PWD/../../bin/debug
        DEFINES += SMF_DISABLE_TRACE_LOG
    }   else {
        LIBS += -lsmf -lglvr
        DESTDIR = $$PWD/../../bin/release
        DEFINES += SMF_DISABLE_DEBUG_LOG SMF_DISABLE_TRACE_LOG NDEBUG
}

win32 {
    LIBS += -L$$PWD/../../dependencies/lib/vc2013/x64/ffmpeg
    LIBS += -lPdh -lUser32
}

unix {
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/glew -lGLEW
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/log4cplus -llog4cplus
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/ffmpeg
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/live555
}
