#ifndef FRAMESTATISTICS_H
#define FRAMESTATISTICS_H

#include <SFML/System.hpp>

class FrameStatistics
{
public:
    FrameStatistics(void);

    void update(void);
    void update(size_t dataSize);

    float getFPS(void) const;

    float getAverageFPS(void) const;

    size_t getBitrate(void) const;

    long getCurrentTimeMilliSecond(void) const;

    void restart(void);


private:
    void caculateFramerate(long elapse);
    void caculateBitrate(long elapse);

private:
    size_t m_FrameCount;
    float m_FPS;
    long m_PrevousTime;
    size_t m_AccumulatedFrames;
    long m_AccumulatedTimeElapsed;
    float m_AverageFPS;
    size_t m_DataSize;
    size_t m_Bitrate;

    sf::Clock m_Clock;
};

#endif // FRAMESTATISTICS_H
