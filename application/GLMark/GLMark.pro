#-------------------------------------------------
#
# Project created by QtCreator 2015-05-26T14:13:20
#
#-------------------------------------------------

QT      += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = GLMark
TEMPLATE = app

CONFIG  += c++11
DEFINES += GLEW_NO_GLU

HEADERS += \
           GLMark.h \
           ScreenSetter.h \
    ../../sample/common/CommonConfig.h \
    ../../sample/common/CPUUsage.h \
    ../../sample/common/MemoryUsage.h \
    ../../sample/common/ViewPane.h \
    ../../sample/common/GLVRWidget.h \
    ../../sample/common/XMLManager.h \
    ../../sample/common/Screenshot.h \
    ../../sample/common/GLMarkLog.h \
    ../../sample/common/RenderingVideoWindow.h \
    ../../sample/common/RenderingWindow.h \
    ../../sample/common/SimpleMediaPlayerWindow.h \
    ../../sample/common/ViewTile.h

SOURCES += main.cpp \
           GLMark.cpp \
           ScreenSetter.cpp \
    ../../sample/common/CPUUsage.cpp \
    ../../sample/common/MemoryUsage.cpp \
    ../../sample/common/ViewPane.cpp \
    ../../sample/common/XMLManager.cpp \
    ../../sample/common/Screenshot.cpp \
    ../../sample/common/GLMarkLog.cpp \
    ../../sample/common/RenderingVideoWindow.cpp \
    ../../sample/common/RenderingWindow.cpp \
    ../../sample/common/SimpleMediaPlayerWindow.cpp \
    ../../sample/common/ViewTile.cpp

FORMS   += \
           GLMark.ui \
    ../../sample/common/RenderingVideoWindow.ui \
    ../../sample/common/SimpleMediaPlayerWindow.ui

INCLUDEPATH += $$PWD/../../dependencies/include
INCLUDEPATH += $$PWD/../../include
DEPENDPATH  += $$PWD/../../include
INCLUDEPATH += $$PWD/../../include/renderer/VideoRenderer
DEPENDPATH  += $$PWD/../../include/renderer/VideoRenderer
INCLUDEPATH += $$PWD/../../sample/common
DEPENDPATH += $$PWD/../../sample/common
LIBS += -L$$PWD/../../lib

CONFIG(debug, debug|release) {
        LIBS += -lsmf-d -lglvr-d
        DESTDIR = $$PWD/../../bin/debug
        DEFINES += SMF_DISABLE_TRACE_LOG
    } else {
        LIBS += -lsmf -lglvr
        DESTDIR = $$PWD/../../bin/release
        DEFINES += SMF_DISABLE_DEBUG_LOG SMF_DISABLE_TRACE_LOG NDEBUG
}

win32 {

    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../../../../../dependencies/lib/vc2013/x64/debug/sfml
        LIBS += -lopengl32 -lPdh -lUser32
    }   else {

        LIBS += -L$$PWD/../../../../../dependencies/lib/vc2013/x64/release/sfml
        LIBS += -lopengl32 -lPdh -lUser32
    }

    LIBS += -L$$PWD/../../dependencies/lib/vc2013/x64/ffmpeg
}

unix {
    QT       +=  x11extras

    HEADERS += ../../sample/RenderingVideo/XVideoRenderer.h
    SOURCES += ../../sample/RenderingVideo/XVideoRenderer.cpp

    LIBS += -L$$PWD/../../dependencies/lib/gcc64/glew -lGLEW
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/log4cplus -llog4cplus
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/ffmpeg
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/live555
    LIBS += -lXv -lX11 -lXrandr -lstdc++
}
