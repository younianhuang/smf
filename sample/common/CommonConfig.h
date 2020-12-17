#ifndef GLMARKCONFIG_H
#define GLMARKCONFIG_H

#include <framework/SMFPlatform.h>

// include

#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QXmlStreamWriter>

#include <fstream>
#include <iostream>
#include <iomanip>

#ifdef SMF_PLATFORM_WIN32

#include <Windows.h> // system info
#include <Psapi.h> // process info
#include <TCHAR.h>
#include <pdh.h>

#elif defined(SMF_PLATFORM_LINUX)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <sys/vtimes.h>

#endif

#include <GLVR.h> // Before Xlib.h for identifier "Status"

// define
#ifdef SMF_PLATFORM_WIN32

    #define CPU_TIME_TYPE ULARGE_INTEGER

#elif defined(SMF_PLATFORM_LINUX)

    #define CPU_TIME_TYPE clock_t

#endif

// XVideo
#define PROCESS_VIDEO_GLVR 0 // OpenGL Video Renderer
#define PROCESS_VIDEO_XVR  1 // XVideo Video Renderer

#ifdef SMF_PLATFORM_LINUX
    #define PROCESS_VIDEO_CONF PROCESS_VIDEO_GLVR
#else
    #define PROCESS_VIDEO_CONF PROCESS_VIDEO_GLVR
#endif // SMF_PLATFORM_LINUX

#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)
    #include "../RenderingVideo/XVideoRenderer.h"
#endif // PROCESS_VIDEO_CONF

#define NO_PARENT       0
#define HIDE_MENU_BAR   true
#define HIDE_STATUS_BAR true
#define TEXTURE_UPLOAD  true
#define SHOW_PROCESS_ID true

#define SHOW_MESSAGE_BOX true

#define TOTAL_TIME      60
#define STATISTICS_TIME 10
#define EXPORT_TIME     TOTAL_TIME*1000+500
#define SHOOT_TIME      TOTAL_TIME*1000+1000
#define CLOSE_TIME      TOTAL_TIME*1000+1500

const surveon::mf::String g_PixelFormatStrings[8] =
{
    "NONE",
    "MJPEG",
    "MPEG4",
    "H264",
    "RGB32",
    "YUV420",
    "YV12",
    "END"
};

const surveon::mf::String g_ResolutionStrings[6] =
{
    "256x256",
    "CIF",
    "512x512",
    "D1",
    "720p",
    "1080p"
};

const int g_ResolutionSize[6 * 2] =
{
    256,  256,
    352,  288,
    512,  512,
    704,  576,
    1280, 720,
    1920, 1080
};

//#define FLOAT_MAX std::numeric_limits<float>::max()
#define FLOAT_MAX 100.0

#define SLEEP(n) std::this_thread::sleep_for(std::chrono::seconds(n))

// log
#include "GLMarkLog.h"

// warning flags
#ifdef SMF_PLATFORM_LINUX
    #pragma GCC diagnostic ignored "-Wunused-function"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-result"
    #pragma GCC diagnostic ignored "-Wsign-compare"
    #pragma GCC diagnostic ignored "-Wreorder"
    #pragma GCC diagnostic ignored "-Wparentheses"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #pragma GCC diagnostic ignored "-Wempty-body"
    #pragma GCC diagnostic ignored "-Wswitch"
#else
    #pragma warning(disable: 4390) // -Wempty-body
#endif


// operator overloading <<
static std::ostream& operator<<(std::ostream& str, const QString& qstr)
{
    return str << qstr.toStdString();
}

// iterator
#define ALL_VIEWTILES for(ViewTiles::iterator it=m_ViewTiles.begin(); it!=m_ViewTiles.end(); ++it) (*it)->
#define ALL_LABELS for (Labels::iterator it=m_Labels.begin(); it!=m_Labels.end(); ++it) (*it)->

#endif // GLMARKCONFIG_H

