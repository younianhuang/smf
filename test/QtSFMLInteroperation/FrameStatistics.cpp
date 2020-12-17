#include "FrameStatistics.h"

FrameStatistics::FrameStatistics(void) :
    m_FrameCount(0),
    m_FPS(0.0f),
    m_PrevousTime(0),
    m_AccumulatedFrames(0),
    m_AccumulatedTimeElapsed(0),
    m_AverageFPS(0.0f),
    m_DataSize(0),
    m_Bitrate(0)
{

}

void FrameStatistics::update(void)
{
    ++m_FrameCount;

    if(m_PrevousTime == 0)
    {
        m_PrevousTime = getCurrentTimeMilliSecond();
    }

    long currentTime = getCurrentTimeMilliSecond();
    long elapse = currentTime - m_PrevousTime;

    if(elapse >=1000)
    {
        caculateFramerate(elapse);

        m_PrevousTime = currentTime;
    }
}

void FrameStatistics::update(size_t dataSize)
{
    ++m_FrameCount;
    m_DataSize += dataSize;

    if(m_PrevousTime == 0)
    {
        m_PrevousTime = getCurrentTimeMilliSecond();
    }

    long currentTime = getCurrentTimeMilliSecond();
    long elapse = currentTime - m_PrevousTime;

    if(elapse >=1000)
    {
        caculateFramerate(elapse);

        caculateBitrate(elapse);

        m_PrevousTime = currentTime;
    }
}

float FrameStatistics::getFPS(void) const
{
    return m_FPS;
}

float FrameStatistics::getAverageFPS(void) const
{
    return m_AverageFPS;
}

size_t FrameStatistics::getBitrate(void) const
{
    return m_Bitrate;
}

long FrameStatistics::getCurrentTimeMilliSecond(void) const
{
    sf::Time time = m_Clock.getElapsedTime();

    return time.asMilliseconds();
}

void FrameStatistics::restart(void)
{
    m_FrameCount = 0;
    m_FPS = 0.0f;
    m_PrevousTime = 0;
    m_AccumulatedFrames = 0;
    m_AccumulatedTimeElapsed = 0;

    m_Clock.restart();
}

void FrameStatistics::caculateFramerate(long elapse)
{
    m_FPS = static_cast<float>(m_FrameCount)*1000.0f/elapse;

    m_AccumulatedFrames += m_FrameCount;
    m_AccumulatedTimeElapsed += elapse;

    if(m_AccumulatedTimeElapsed >= 5000)
    {
        m_AverageFPS = static_cast<float>(m_AccumulatedFrames)*1000.0f/m_AccumulatedTimeElapsed;

        m_AccumulatedFrames = 0;
        m_AccumulatedTimeElapsed = 0;
    }

    m_FrameCount = 0;
}

void FrameStatistics::caculateBitrate(long elapse)
{
    m_Bitrate = (m_DataSize << 3) * 1000 / elapse;

    m_DataSize = 0;
}

