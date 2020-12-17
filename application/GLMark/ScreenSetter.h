#ifndef SCREENSETTER_H
#define SCREENSETTER_H

#include "CommonConfig.h"

#ifdef SMF_PLATFORM_LINUX

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Avoid compilation error befor Xlib.h
#include <QtCore/qcoreevent.h>
#include <QtWidgets/qstyleoption.h>
#include <QtCore/qurl.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#ifdef Bool
#undef Bool // Because of "#define Bool Int" in Xlib.h
#endif

#endif

class ScreenSetter
{
public:
    ScreenSetter();
    ~ScreenSetter();

    int getNumSizes() const;
    int getNumRates(unsigned int resolutionIndex) const;
    int getFrequency(unsigned int resolutionIndex, unsigned int frequencyIndex) const;
    int getCurrentSizeID();
    int getCurrentRate();
    int getCurrentRateID();
    int getScreenSizeWidth(unsigned int resolutionIndex) const;
    int getScreenSizeHeight(unsigned int resolutionIndex) const;
    void setScreenResolution(unsigned int resolutionIndex = 0, unsigned int frequencyIndex = 0);

private:
    int                     m_NumSizes;
    short                   m_PossibleFrequencies[64][64];
#ifdef SMF_PLATFORM_LINUX
    short                   m_CurrentRate;
    SizeID                  m_CurrentSizeID;
    Display*                m_pDisplay;
    Window                  m_Root;
    XRRScreenSize*          m_pScreenSize;
    XRRScreenConfiguration* m_Conf;
    Rotation                m_CurrentRotation;
#elif defined(SMF_PLATFORM_WIN32)
    int                     m_CurrentSizeID;
    int                     m_ScreenSize[64][2];
    int                     m_NumRates[64];
#endif
};

#endif // SCREENSETTER_H
