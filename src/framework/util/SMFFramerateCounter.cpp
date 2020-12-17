

#include "framework/util/SMFFramerateCounter.h"


namespace surveon
{
namespace mf
{

FramerateCounter::FramerateCounter(int64 sampleMilliseconds) :
    m_FrameCount(0),
    m_LastTime(0),
    m_SampleDuration(sampleMilliseconds),
    m_FPS(0.0f)
{

}

void FramerateCounter::reset()
{
    m_FrameCount = 0;
    m_LastTime = 0;
    m_FPS = 0.0f;
}

bool FramerateCounter::update(int64 nowMilliseconds)
{
    m_FrameCount++;

    if(m_LastTime == 0)
        m_LastTime = nowMilliseconds;

    int64 elapse = nowMilliseconds - m_LastTime;
    if( elapse> m_SampleDuration)
    {
        m_FPS = static_cast<float>(m_FrameCount * 1000)/elapse;
        m_FrameCount = 0;
        m_LastTime = nowMilliseconds;

        return true;
    }

    return false;
}


} // namespace mf
} // namespace surveon




