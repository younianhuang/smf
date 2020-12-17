

#include "framework/util/SMFReferenceCount.h"


namespace surveon
{
namespace mf
{

ReferenceCount::ReferenceCount(void) : m_Count(0)
{

}

ReferenceCount::~ReferenceCount(void)
{
    RecursiveMutexLock lock(m_Mutex);
    m_Count = 0;

    lock.unlock();
    m_ConditionVariable.notify_all();
}

bool ReferenceCount::isReferenced(void) const
{
    return m_Count>0;
}

void ReferenceCount::addReference(void)
{
    SMF_MUTEX_LOCK(m_Mutex);
    ++m_Count;
}

void ReferenceCount::releaseReference(void)
{
    RecursiveMutexLock lock(m_Mutex);

    if(m_Count>0)
    {
        --m_Count;
        if(m_Count == 0)
        {
            lock.unlock();
            m_ConditionVariable.notify_all();
        }
    }
}

void ReferenceCount::waitUntilFree(void)
{
    RecursiveMutexLock lock(m_Mutex);
    while(m_Count > 0)
    {
        m_ConditionVariable.wait(lock);
    }
}


} // namespace mf
} // namespace surveon





