#ifndef SMF_HIGH_RESOLUTION_ClockImpl_H
#define SMF_HIGH_RESOLUTION_ClockImpl_H

#include "framework/pipeline/SMFIClock.h"
#include <chrono>

namespace surveon
{
namespace mf
{

template<class ClockType>
class ClockImpl : public IClock
{
public:
    virtual ~ClockImpl(void)    {}

    virtual milli_seconds getNowMilliSeconds(void) const
    {
        std::chrono::time_point<ClockType> now = ClockType::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }

    virtual micro_seconds getNowMicroSeconds(void) const
    {
        std::chrono::time_point<ClockType> now = ClockType::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    }

    virtual nano_seconds getNowNanoSeconds(void) const
    {
        std::chrono::time_point<ClockType> now = ClockType::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }
};


typedef ClockImpl<std::chrono::system_clock> SystemClock;
typedef ClockImpl<std::chrono::high_resolution_clock> HighResolutionClock;

} // namespace mf
} // namespace surveon




#endif // SMF_HIGH_RESOLUTION_ClockImpl_H




