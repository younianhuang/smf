

#include "framework/util/SMFBitrateCounter.h"


namespace surveon
{
namespace mf
{

BitrateCounter::BitrateCounter(int64 sampleMilliseconds) :
    m_LastTime(0),
    m_SampleDuration(sampleMilliseconds),
    m_DataCount(0),
    m_Bitrate(0)
{

}

void BitrateCounter::reset()
{
    m_LastTime = 0;
    m_DataCount = 0;
    m_Bitrate = 0;
}

void BitrateCounter::accumulate(uint64 dataSizeInBits)
{
    m_DataCount += dataSizeInBits;
}

bool BitrateCounter::update(int64 nowMilliseconds)
{
    if(m_LastTime == 0)
        m_LastTime = nowMilliseconds;

    int64 elapse = nowMilliseconds - m_LastTime;
    if( elapse> m_SampleDuration)
    {
        m_Bitrate = (m_DataCount * 1000)/elapse;
        m_DataCount = 0;
        m_LastTime = nowMilliseconds;

        return true;
    }

    return false;
}


} // namespace mf
} // namespace surveon





