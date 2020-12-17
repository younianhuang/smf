#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T10:11:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSFMLInteroperation
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SFMLRenderWindow.cpp \
    FrameStatistics.cpp

HEADERS  += MainWindow.h \
    SFMLRenderWindow.h \
    FrameStatistics.h

FORMS    += MainWindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dependencies/lib/vc2013/x64/release/sfml -lsfml-main -lsfml-window -lsfml-system -lsfml-graphics -lglu32 -lopengl32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dependencies/lib/vc2013/x64/debug/sfml -lsfml-main-d -lsfml-window-d -lsfml-system-d -lsfml-graphics-d  -lglu32 -lopengl32
else:unix:!macx: LIBS += -L$$PWD/../../dependencies/lib/gcc64/sfml -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system -lX11 -lGL -lGLU -L$$PWD/../../dependencies/lib/gcc64/glew -lGLEW

INCLUDEPATH += $$PWD/../../dependencies/include
DEPENDPATH += $$PWD/../../dependencies/include
