#ifndef SMF_FRAMERATE_COUNTER_H
#define SMF_FRAMERATE_COUNTER_H

#include "framework/SMFPrerequisites.h"

namespace surveon
{
namespace mf
{

class SMF_API FramerateCounter
{
public:
    FramerateCounter(int64 sampleMilliseconds = 1000);

    float getFPS(void) const
    {
        return m_FPS;
    }

    bool update(int64 nowMilliseconds);

    void reset();

private:
    size_t m_FrameCount;
    int64 m_LastTime;
    int64 m_SampleDuration;
    float m_FPS;
};


} // namespace mf
} // namespace surveon




#endif // SMF_FRAMERATE_COUNTER_H

