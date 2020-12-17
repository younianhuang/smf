
#include "GLVRPixmapVideoBuffer.h"


namespace surveon
{
namespace glvr
{

HardwareVideoBuffer::HardwareVideoBuffer(HardwarePixelBufferSharedPtr pPixmap) :
    m_MaxLength(0),
    m_Locked(false),
    m_pHardwarePixelBuffer(pPixmap)

{
    assert(pPixmap);

    PixelFormatDescription pfDesc = PixelFormatDescriptor::getPixelFormatDescription(SMF_VIDEO_FORMAT_RGBA);

    int pitch = static_cast<int>(pPixmap->getWidth() * (pfDesc.bitsPerPixel >> 3));
    uint32 size = pitch * pPixmap->getHeight();

    // TODO:
    //pixel pointer
    m_PlaneInfo.addPlane( PlaneInfo::PlaneData(
                              nullptr,
                              pPixmap->getWidth(),
                              pPixmap->getHeight() ,
                              pitch,
                              size));
}

HardwareVideoBuffer:: ~HardwareVideoBuffer(void)
{
    m_pHardwarePixelBuffer.reset();
}

size_t HardwareVideoBuffer::getMaxLength(void) const
{
    return m_MaxLength;
}

void HardwareVideoBuffer::setCurrentLength(size_t length)
{

}

size_t HardwareVideoBuffer::getCurrentLength(void) const
{
    return m_MaxLength;
}

uint8* HardwareVideoBuffer::lock(void)
{
    m_Locked = true;
    return 0;
}
void HardwareVideoBuffer::unlock(void)
{
    m_Locked = false;
}

bool HardwareVideoBuffer::isLocked(void) const
{
    return m_Locked;
}

bool HardwareVideoBuffer::isReferenced(void) const
{
    return m_ReferenceCount.isReferenced();
}

void HardwareVideoBuffer::setReferenced(bool valid)
{        
    if(valid)
        m_ReferenceCount.addReference();
    else
        m_ReferenceCount.releaseReference();
}

void HardwareVideoBuffer::waitUntilFree(void)
{
    m_ReferenceCount.waitUntilFree();
}

void HardwareVideoBuffer::setWidth(const int width)
{

}

int HardwareVideoBuffer::getWidth(void) const
{
    return m_pHardwarePixelBuffer->getWidth();
}

void HardwareVideoBuffer::setHeight(const int height)
{

}


 int HardwareVideoBuffer::getHeight(void) const
 {
     return m_pHardwarePixelBuffer->getHeight();
 }

MediaSubType HardwareVideoBuffer::getVideoFormat(void) const
{
    return m_pHardwarePixelBuffer->getFormat();
}

const PlaneInfo& HardwareVideoBuffer::lockPlane(void)
{
    m_Locked = true;
    return m_PlaneInfo;
}

void HardwareVideoBuffer::unlockPlane(void)
{
    m_Locked = false;
}

void HardwareVideoBuffer::setTimestamp(micro_seconds timestamp)
{
    m_Timestamp = timestamp;
}

micro_seconds HardwareVideoBuffer::getTimestamp(void) const
{
    return m_Timestamp;
}

void* HardwareVideoBuffer::getPrivateData(void)
{
    assert(m_pHardwarePixelBuffer);
    return m_pHardwarePixelBuffer->getPrivateData();
}

Attributes* HardwareVideoBuffer::getAttributes(void)
{
    return nullptr;
}

//=================================================================================



} // namespace mf
} // namespace surveon

