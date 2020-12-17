
#ifndef SMF_IPRESENTATION_CLOCK_H
#define SMF_IPRESENTATION_CLOCK_H


#include "framework/SMFPrerequisites.h"
#include "framework/pipeline/SMFIClock.h"


namespace surveon
{
namespace mf
{

class IPresentationClock
{
public:
    virtual ~IPresentationClock(void)    {}

    virtual void start(micro_seconds startOffset = 0) = 0;

    virtual void stop(void) = 0;

    virtual void pause(void) = 0;

    virtual void resume(void) = 0;

    virtual micro_seconds getPresentationTime(void) = 0;

    virtual void setRate(float rate) = 0;

    virtual void setTimeSource(IClock* pClock) = 0;
};


} // namespace mf
} // namespace surveon


#endif // SMF_IPRESENTATION_CLOCK_H

