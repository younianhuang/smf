#include "SMFPresentationClockImpl.h"

namespace surveon
{
namespace mf
{

PresentationClockImpl::PresentationClockImpl() :
    m_pTimeSource(nullptr),
    m_Rate(1.0f),
    m_LastSystemTime(0),
    m_LastPresentationTime(0),
    m_State(ST_STOPPED)
{

}

void PresentationClockImpl::start(micro_seconds startOffset)
{
    SMF_MUTEX_LOCK(m_Mutex);

    SMF_CORE_LOG_DEBUG("start ");

    if(!m_pTimeSource)
    {
        SMF_CORE_THROW_EXCEPT(InvalidOperationException, "TimeSource does not exist");
    }

    m_LastSystemTime = m_pTimeSource->getNowMicroSeconds();

    m_LastPresentationTime = startOffset;

    SMF_CORE_LOG_DEBUG("m_LastSystemTime = " << m_LastSystemTime <<
                    " m_LastPresentationTime = " << m_LastPresentationTime);

     m_State = ST_RUNNING;
}

void PresentationClockImpl::stop(void)
{
    SMF_MUTEX_LOCK(m_Mutex);
    m_State = ST_STOPPED;
}

void PresentationClockImpl::pause(void)
{
    SMF_MUTEX_LOCK(m_Mutex);

    if(m_State != ST_RUNNING)
    {
        SMF_CORE_THROW_EXCEPT(InvalidStateException, "This clock is running");
    }

    m_State = ST_PAUSE;
}

void PresentationClockImpl::resume(void)
{
    SMF_MUTEX_LOCK(m_Mutex);

    if(m_State != ST_PAUSE)
    {
        SMF_CORE_THROW_EXCEPT(InvalidStateException, "This clock is not pause");
    }
    m_State = ST_RUNNING;
}


micro_seconds PresentationClockImpl::getPresentationTime(void)
{
    SMF_MUTEX_LOCK(m_Mutex);

    if(!m_pTimeSource)
    {
        SMF_CORE_THROW_EXCEPT(InvalidOperationException, "TimeSource does not exist");
    }

    if(m_State == ST_STOPPED)
        return 0;

    micro_seconds now = m_pTimeSource->getNowMicroSeconds();

    if(m_State == ST_RUNNING)
    {
        m_LastPresentationTime +=
                static_cast<time_value>((now - m_LastSystemTime) * m_Rate);
    }

    m_LastSystemTime = now;

    return m_LastPresentationTime;
}


void PresentationClockImpl::setRate(float rate)
{
    SMF_MUTEX_LOCK(m_Mutex);
    m_Rate = rate;
}

void PresentationClockImpl::setTimeSource(IClock* pClock)
{
    SMF_MUTEX_LOCK(m_Mutex);
    m_pTimeSource = pClock;
}

} // namespace mf
} // namespace surveon
