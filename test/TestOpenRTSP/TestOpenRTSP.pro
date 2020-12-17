

TARGET = TestOpenRTSP
TEMPLATE = app

CONFIG += c++11

HEADERS += \
    playCommon.hh \
    strDup.h \
    Boolean.h \
    Base64.h

SOURCES += \
    playCommon.cpp \
    openRTSP.cpp \
    strDup.cpp \
    Base64.cpp



INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
INCLUDEPATH += $$PWD/../../dependencies/include/live555/BasicUsageEnvironment
DEPENDPATH += $$PWD/../../dependencies/include/live555/BasicUsageEnvironment
INCLUDEPATH += $$PWD/../../dependencies/include/live555/liveMedia
DEPENDPATH += $$PWD/../../dependencies/include/live555/liveMedia
INCLUDEPATH += $$PWD/../../dependencies/include/live555/UsageEnvironment
DEPENDPATH += $$PWD/../../dependencies/include/live555/UsageEnvironment
INCLUDEPATH += $$PWD/../../dependencies/include/live555/groupsock
DEPENDPATH += $$PWD/../../dependencies/include/live555/groupsock

unix {
    LIBS += -L$$PWD/../../dependencies/lib/gcc64/live555 -lBasicUsageEnvironment -lliveMedia -lUsageEnvironment -lgroupsock
}

