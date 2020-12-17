
#include "framework/primitives/SMFWrapperMediaBuffer.h"

namespace surveon
{
namespace mf
{

WrapperMediaBuffer::WrapperMediaBuffer(void) :
    m_pBuffer(nullptr),
    m_MaxLength(0),
    m_Valid(false),
    m_Locked(false)
{

}

WrapperMediaBuffer:: ~WrapperMediaBuffer(void)
{
    setReferenced(false);
}

size_t WrapperMediaBuffer::getMaxLength(void) const
{
    return m_MaxLength;
}

void WrapperMediaBuffer::setCurrentLength(size_t length)
{
    m_CurrentLength = length;
}

size_t WrapperMediaBuffer::getCurrentLength(void) const
{
    return m_CurrentLength;
}

uint8* WrapperMediaBuffer::lock(void)
{
    m_Locked = true;
    return m_pBuffer;
}
void WrapperMediaBuffer::unlock(void)
{
    m_Locked = false;
}

bool WrapperMediaBuffer::isLocked(void) const
{
    return m_Locked;
}

bool WrapperMediaBuffer::isReferenced(void) const
{
    return m_ReferenceCount.isReferenced();
}

void WrapperMediaBuffer::setReferenced(bool valid)
{
    if(valid)
        m_ReferenceCount.addReference();
    else
        m_ReferenceCount.releaseReference();
}

void WrapperMediaBuffer::waitUntilFree(void)
{
    m_ReferenceCount.waitUntilFree();
}

void WrapperMediaBuffer::setTimestamp(micro_seconds timestamp)
{
    m_Timestamp = timestamp;
}

micro_seconds WrapperMediaBuffer::getTimestamp(void) const
{
    return m_Timestamp;
}

void* WrapperMediaBuffer::getPrivateData(void)
{
    return nullptr;
}

Attributes*  WrapperMediaBuffer::getAttributes(void)
{
    return nullptr;
}

} // namespace mf
} // namespace surveon

