#ifndef SMF_BITRATE_COUNTER_H
#define SMF_BITRATE_COUNTER_H

#include "framework/SMFPrerequisites.h"

namespace surveon
{
namespace mf
{

class SMF_API BitrateCounter
{
public:
    BitrateCounter(int64 sampleMilliseconds = 1000);

    uint64 getBitrate(void) const
    {
        return m_Bitrate;
    }

    void accumulate(uint64 dataSizeInBits);

    bool update( int64 nowMilliseconds);

    void reset();

private:
    int64 m_LastTime;
    int64 m_SampleDuration;
    uint64 m_DataCount;
    uint64 m_Bitrate;
};


} // namespace mf
} // namespace surveon



#endif // SMF_BITRATE_COUNTER_H

