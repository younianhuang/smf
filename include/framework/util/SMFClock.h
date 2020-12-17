#ifndef SMF_CLOCK_H
#define SMF_CLOCK_H

#include "framework/SMFPrerequisites.h"
#include <chrono>

namespace surveon
{
namespace mf
{

class SMF_API Clock
{
public:
    Clock(void);

    void restart(void);

    milli_seconds getMilliSeconds(void) const;

    micro_seconds getMicroSeconds(void) const;

private:
    std::chrono::high_resolution_clock::time_point m_Start;

};


} // namespace mf
} // namespace surveon




#endif // SMF_CLOCK_H



