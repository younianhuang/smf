#ifndef SMF_PRESENTATION_CLOCKIMPL_H
#define SMF_PRESENTATION_CLOCKIMPL_H

#include "SMFCorePrerequisites.h"
#include "framework/pipeline/SMFIPresentationClock.h"

namespace surveon
{
namespace mf
{

class PresentationClockImpl : public IPresentationClock
{
public:
    enum State
    {
        ST_STOPPED,
        ST_RUNNING,
        ST_PAUSE
    };

public:
    PresentationClockImpl(void);

    virtual ~PresentationClockImpl(void)    {}

    virtual void start(micro_seconds startOffset = 0);

    virtual void stop(void);

    virtual void pause(void);

    virtual void resume(void);

    virtual micro_seconds getPresentationTime(void);

    virtual void setRate(float rate);

    virtual void setTimeSource(IClock* pClock);

private:
    IClock* m_pTimeSource;

    float m_Rate;

    micro_seconds m_LastSystemTime;

    micro_seconds m_LastPresentationTime;

    State m_State;

    mutable Mutex m_Mutex;
};


} // namespace mf
} // namespace surveon



#endif // SMF_PRESENTATION_CLOCKIMPL_H
