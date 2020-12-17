
#include "framework/primitives/SMFWrapperVideoBuffer.h"

namespace surveon
{
namespace mf
{

WrapperVideoBuffer::WrapperVideoBuffer(void) :
    m_Width(0),
    m_Height(0),
    m_PixelFormat(SMF_SUB_TYPE_NONE)
{

}


WrapperVideoBuffer::WrapperVideoBuffer(int width, int height, MediaSubType format) :
    m_Width(width),
    m_Height(height),
    m_PixelFormat(format)
{

}

WrapperVideoBuffer::~WrapperVideoBuffer(void)
{

}

size_t WrapperVideoBuffer::getMaxLength(void) const
{
    return m_MediaBuffer.getMaxLength();
}

void WrapperVideoBuffer::setCurrentLength(size_t length)
{
    m_MediaBuffer.setCurrentLength(length);
}

size_t WrapperVideoBuffer::getCurrentLength(void) const
{
    return m_MediaBuffer.getCurrentLength();
}

uint8* WrapperVideoBuffer::lock(void)
{
    return m_MediaBuffer.lock();
}
void WrapperVideoBuffer::unlock(void)
{
    m_MediaBuffer.unlock();
}

bool WrapperVideoBuffer::isLocked(void) const
{
    return m_MediaBuffer.isLocked();
}

bool WrapperVideoBuffer::isReferenced(void) const
{
    return m_MediaBuffer.isReferenced();
}

void WrapperVideoBuffer::setReferenced(bool valid)
{
    m_MediaBuffer.setReferenced(valid);
}

void WrapperVideoBuffer::waitUntilFree(void)
{
    m_MediaBuffer.waitUntilFree();
}

void WrapperVideoBuffer::setWidth(const int width)
{
    m_Width = width;
}

int WrapperVideoBuffer::getWidth(void) const
{
    return m_Width;
}

void WrapperVideoBuffer::setHeight(const int height)
{
    m_Height = height;
}


 int WrapperVideoBuffer::getHeight(void) const
 {
     return m_Height;
 }

 void WrapperVideoBuffer::setPixelFormat(MediaSubType format)
 {
     if(m_PixelFormat != format)
     {
        m_PixelFormat = format;
        m_PlaneInfo.clearPlaneDatas();
     }
 }

MediaSubType WrapperVideoBuffer::getVideoFormat(void) const
{
    return m_PixelFormat;
}

const PlaneInfo& WrapperVideoBuffer::lockPlane(void)
{
    m_MediaBuffer.lock();
    return m_PlaneInfo;
}

void WrapperVideoBuffer::unlockPlane(void)
{
    m_MediaBuffer.unlock();
}

void WrapperVideoBuffer::setTimestamp(micro_seconds timestamp)
{
    m_MediaBuffer.setTimestamp(timestamp);
}

micro_seconds WrapperVideoBuffer::getTimestamp(void) const
{
    return m_MediaBuffer.getTimestamp();
}

void* WrapperVideoBuffer::getPrivateData(void)
{
    return m_MediaBuffer.getPrivateData();
}

Attributes*  WrapperVideoBuffer::getAttributes(void)
{
    return nullptr;
}
//=================================================================================
void WrapperVideoBuffer::setBufferPtr(uint8* buffer)
{    
    m_MediaBuffer.setBufferPtr(buffer);

    const PixelFormatDescription& pfDesc = PixelFormatDescriptor::getPixelFormatDescription(m_PixelFormat);

    uint32 allocateSize = 0;

    m_PlaneInfo.clearPlaneDatas();

    for(auto& planeFormat : pfDesc.planeFormatDescriptions )
    {
        int width = m_Width >> (planeFormat.widthDiv-1);
        int height = m_Height >> (planeFormat.heightDiv-1);
        int pitch = static_cast<int>(width * (pfDesc.bitsPerPixel >> 3));
        uint32 size = pitch * height;

        m_PlaneInfo.addPlane( PlaneInfo::PlaneData(
                                  nullptr,
                                  width,
                                  height ,
                                  pitch,
                                  size));

        allocateSize += size;
    }

    m_PlaneInfo.setPixelBufferPtr(buffer);
}


} // namespace mf
} // namespace surveon
