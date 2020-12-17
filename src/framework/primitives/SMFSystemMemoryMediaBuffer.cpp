
#include "SMFSystemMemoryMediaBuffer.h"

namespace surveon
{
namespace mf
{

SystemMemoryMediaBuffer::SystemMemoryMediaBuffer(void) :
    m_pBuffer(nullptr),
    m_MaxLength(0),
    m_Referenced(false),
    m_Locked(false),
    m_Timestamp(0)
{

}

SystemMemoryMediaBuffer::SystemMemoryMediaBuffer(size_t size, uint8 aligment) :
    m_pBuffer(nullptr)
{
    allocate(size, aligment);
}

SystemMemoryMediaBuffer:: ~SystemMemoryMediaBuffer(void)
{
    deallocate();
    setReferenced(false);
}

void SystemMemoryMediaBuffer::allocate(size_t size, uint8 aligment)
{    
    //SMF_MUTEX_LOCK(m_Mutex);
    // TODO: aligment

    m_pBuffer.reset(new uint8[size]);
    m_MaxLength = size;
}

void SystemMemoryMediaBuffer::deallocate(void)
{
    //SMF_MUTEX_LOCK(m_Mutex);
    m_pBuffer.reset();    
    m_MaxLength = 0;
    m_CurrentLength = 0;
}

size_t SystemMemoryMediaBuffer::getMaxLength(void) const
{
    return m_MaxLength;
}

void SystemMemoryMediaBuffer::setCurrentLength(size_t length)
{
    m_CurrentLength = length;
}

size_t SystemMemoryMediaBuffer::getCurrentLength(void) const
{
    return m_CurrentLength;
}

uint8* SystemMemoryMediaBuffer::lock(void)
{    
    //m_Mutex.lock();
    m_Locked = true;
    return m_pBuffer.get();
}
void SystemMemoryMediaBuffer::unlock(void)
{
    //m_Mutex.unlock();
    m_Locked = false;
}

bool SystemMemoryMediaBuffer::isLocked(void) const
{
    return m_Locked;
}

bool SystemMemoryMediaBuffer::isReferenced(void) const
{
    return m_ReferenceCount.isReferenced();
}

void SystemMemoryMediaBuffer::setReferenced(bool valid)
{
    if(valid)
        m_ReferenceCount.addReference();
    else
        m_ReferenceCount.releaseReference();
}

void SystemMemoryMediaBuffer::waitUntilFree(void)
{
    m_ReferenceCount.waitUntilFree();
}

void SystemMemoryMediaBuffer::setTimestamp(micro_seconds timestamp)
{
    m_Timestamp = timestamp;
}

micro_seconds SystemMemoryMediaBuffer::getTimestamp(void) const
{
    return m_Timestamp;
}

void* SystemMemoryMediaBuffer::getPrivateData(void)
{
    return nullptr;
}

Attributes* SystemMemoryMediaBuffer::getAttributes(void)
{
    return nullptr;
}

} // namespace mf
} // namespace surveon

