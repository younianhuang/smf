#include "ScreenSetter.h"

#ifdef SMF_PLATFORM_LINUX

ScreenSetter::ScreenSetter()
{
    // Connect to X-Server, get root window ID
    m_pDisplay = XOpenDisplay(NULL);
    m_Root = RootWindow(m_pDisplay, 0);

    // Get possible screen resolutions
    m_pScreenSize = XRRSizes(m_pDisplay, 0, &m_NumSizes);

    // Loop through all possible resolutions, getting the selectable display frequencies
    for (int i=0; i<m_NumSizes; i++)
    {
        short* pRates;
        int    numRates;

        pRates = XRRRates(m_pDisplay, 0, i, &numRates);

        for (int j=0; j<numRates; j++)
        {
            m_PossibleFrequencies[i][j] = pRates[j];
        }
    }

    // Get current resolution and frequency
    m_Conf = XRRGetScreenInfo(m_pDisplay, m_Root);
    m_CurrentRate = XRRConfigCurrentRate(m_Conf);
    m_CurrentSizeID = XRRConfigCurrentConfiguration(m_Conf, &m_CurrentRotation);
}

ScreenSetter::~ScreenSetter()
{
    // Exit
    XCloseDisplay(m_pDisplay);
}

int ScreenSetter::getNumSizes() const
{
    return m_NumSizes;
}

int ScreenSetter::getNumRates(unsigned int resolutionIndex) const
{
    int numRates;
    XRRRates(m_pDisplay, 0, resolutionIndex, &numRates);

    return numRates;
}

int ScreenSetter::getFrequency(unsigned int resolutionIndex, unsigned int frequencyIndex) const
{
    return m_PossibleFrequencies[resolutionIndex][frequencyIndex];
}

int ScreenSetter::getCurrentSizeID()
{
    m_CurrentSizeID = XRRConfigCurrentConfiguration(m_Conf, &m_CurrentRotation);

    return (int)m_CurrentSizeID;
}

int ScreenSetter::getCurrentRate()
{
    m_CurrentRate = XRRConfigCurrentRate(m_Conf);

    return (int)m_CurrentRate;
}

int ScreenSetter::getCurrentRateID()
{
    int currentSizeID = getCurrentSizeID();
    int numRates = getNumRates(currentSizeID);
    int currentRate = getCurrentRate();

    for (int i=0; i<numRates; i++)
    {
        if (currentRate == m_PossibleFrequencies[currentSizeID][i])
        {
            return i;
        }
    }

    return -1;
}

int ScreenSetter::getScreenSizeWidth(unsigned int resolutionIndex) const
{
    return m_pScreenSize[resolutionIndex].width;
}

int ScreenSetter::getScreenSizeHeight(unsigned int resolutionIndex) const
{
    return m_pScreenSize[resolutionIndex].height;
}

void ScreenSetter::setScreenResolution(unsigned int resolutionIndex, unsigned int frequencyIndex)
{
    // Change Resolution
    XRRSetScreenConfigAndRate(m_pDisplay, m_Conf, m_Root, resolutionIndex, RR_Rotate_0,
                              m_PossibleFrequencies[resolutionIndex][frequencyIndex], CurrentTime);
}

// Please Refer To:
//
//     https://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_Changing_the_Screen_Resolution
//

#elif defined(SMF_PLATFORM_WIN32)

ScreenSetter::ScreenSetter() :
    m_NumSizes(0)
{
    for (int i=0; i<64; i++)
    {
        m_NumRates[i] = 0;
    }

    for (int i=0; i<64; i++)
    {
        for (int j=0; j<64; j++)
        {
            m_PossibleFrequencies[i][j] = 0;
        }
    }

    for (int i=0; i<64; i++)
    {
        for (int j=0; j<2; j++)
        {
            m_ScreenSize[i][j] = 0;
        }
    }

    DEVMODE DevMode;
    int width=0, height=0, frequency=0;

    for (int i=0; EnumDisplaySettings(NULL, i, &DevMode); i++)
    {
        if ((width == DevMode.dmPelsWidth) && (height == DevMode.dmPelsHeight) && ((unsigned)frequency < DevMode.dmDisplayFrequency))
        {
            width = DevMode.dmPelsWidth;
            height = DevMode.dmPelsHeight;
            frequency = DevMode.dmDisplayFrequency;

            m_PossibleFrequencies[m_NumSizes - 1][m_NumRates[m_NumSizes - 1]] = frequency;
            m_NumRates[m_NumSizes - 1]++;
        }
        else if ((width == DevMode.dmPelsWidth) && (height == DevMode.dmPelsHeight) && ((unsigned)frequency >= DevMode.dmDisplayFrequency))
        {
            continue;
        }
        else if ((width != DevMode.dmPelsWidth) || (height != DevMode.dmPelsHeight) || (frequency != DevMode.dmDisplayFrequency))
        {
            width = DevMode.dmPelsWidth;
            height = DevMode.dmPelsHeight;
            frequency = DevMode.dmDisplayFrequency;

            m_PossibleFrequencies[m_NumSizes][m_NumRates[m_NumSizes]] = frequency;
            m_NumRates[m_NumSizes]++;
            m_ScreenSize[m_NumSizes][0] = width;
            m_ScreenSize[m_NumSizes][1] = height;
            m_NumSizes++;
        }
    }
}

ScreenSetter::~ScreenSetter()
{

}

int ScreenSetter::getNumSizes() const
{
    return m_NumSizes;
}

int ScreenSetter::getNumRates(unsigned int resolutionIndex) const
{
    return m_NumRates[resolutionIndex];
}

int ScreenSetter::getFrequency(unsigned int resolutionIndex, unsigned int frequencyIndex) const
{
    return m_PossibleFrequencies[resolutionIndex][frequencyIndex];
}

int ScreenSetter::getCurrentSizeID()
{
    DEVMODE DevMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);

    for (int i=0; i<m_NumSizes; i++)
    {
        if ((m_ScreenSize[i][0] == DevMode.dmPelsWidth) && (m_ScreenSize[i][1] == DevMode.dmPelsHeight))
        {
            return i;
        }
    }

    return -1;
}

int ScreenSetter::getCurrentRate()
{
    DEVMODE DevMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);

    return DevMode.dmDisplayFrequency;
}

int ScreenSetter::getCurrentRateID()
{
    int currentSizeID = getCurrentSizeID();
    int numRates = getNumRates(currentSizeID);
    int currentRate = getCurrentRate();

    for (int i=0; i<numRates; i++)
    {
        if (currentRate == m_PossibleFrequencies[currentSizeID][i])
        {
            return i;
        }
    }

    return -1;
}

int ScreenSetter::getScreenSizeWidth(unsigned int resolutionIndex) const
{
    return m_ScreenSize[resolutionIndex][0];
}

int ScreenSetter::getScreenSizeHeight(unsigned int resolutionIndex) const
{
    return m_ScreenSize[resolutionIndex][1];
}

void ScreenSetter::setScreenResolution(unsigned int resolutionIndex, unsigned int frequencyIndex)
{
    DEVMODE DevMode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);

    DevMode.dmPelsWidth = m_ScreenSize[resolutionIndex][0];
    DevMode.dmPelsHeight = m_ScreenSize[resolutionIndex][1];
    DevMode.dmDisplayFrequency = m_PossibleFrequencies[resolutionIndex][frequencyIndex];

    ChangeDisplaySettings(&DevMode, 0);
}

#endif

