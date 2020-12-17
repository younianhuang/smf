

#ifndef SMF_ICLOCK_H
#define SMF_ICLOCK_H


#include "framework/SMFPrerequisites.h"


namespace surveon
{
namespace mf
{

enum ClockType
{
    CT_SYSTEM_CLOCK,
    CT_HIGH_RESOLUTION_CLOCK
};

class IClock
{
public:
    virtual ~IClock(void)    {}

    virtual milli_seconds getNowMilliSeconds(void) const = 0;

    virtual micro_seconds getNowMicroSeconds(void) const = 0;

    virtual nano_seconds getNowNanoSeconds(void) const = 0;
};


} // namespace mf
} // namespace surveon


#endif // SMF_ICLOCK_H




