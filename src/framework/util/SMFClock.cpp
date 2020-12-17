

#include "framework/util/SMFClock.h"


namespace surveon
{
namespace mf
{

Clock::Clock(void)

{

}

void Clock::restart()
{
    m_Start = std::chrono::high_resolution_clock::now();
}

milli_seconds Clock::getMilliSeconds(void) const
{
    std::chrono::high_resolution_clock::duration elapse = std::chrono::high_resolution_clock::now() - m_Start;

    return std::chrono::duration_cast<std::chrono::milliseconds>(elapse).count();
}

micro_seconds Clock::getMicroSeconds(void) const
{
    std::chrono::high_resolution_clock::duration elapse = std::chrono::high_resolution_clock::now() - m_Start;

    return std::chrono::duration_cast<std::chrono::microseconds>(elapse).count();
}



} // namespace mf
} // namespace surveon




