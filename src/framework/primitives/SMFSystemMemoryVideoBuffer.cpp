
#include "SMFSystemMemoryVideoBuffer.h"

namespace surveon
{
namespace mf
{

SystemMemoryVideoBuffer::SystemMemoryVideoBuffer(int width, int height, MediaSubType format, uint8 aligment) :
    m_Width(width),
    m_Height(height),
    m_PixelFormat(format)
{
    allocate();
}

SystemMemoryVideoBuffer:: ~SystemMemoryVideoBuffer(void)
{
    deallocate();
}

size_t SystemMemoryVideoBuffer::getMaxLength(void) const
{
    return m_MediaBuffer.getMaxLength();
}

void SystemMemoryVideoBuffer::setCurrentLength(size_t length)
{
    m_MediaBuffer.setCurrentLength(length);
}

size_t SystemMemoryVideoBuffer::getCurrentLength(void) const
{
    return m_MediaBuffer.getCurrentLength();
}

uint8* SystemMemoryVideoBuffer::lock(void)
{
    return m_MediaBuffer.lock();
}
void SystemMemoryVideoBuffer::unlock(void)
{
    m_MediaBuffer.unlock();
}

bool SystemMemoryVideoBuffer::isLocked(void) const
{
    return m_MediaBuffer.isLocked();
}

bool SystemMemoryVideoBuffer::isReferenced(void) const
{
    return m_MediaBuffer.isReferenced();
}

void SystemMemoryVideoBuffer::setReferenced(bool valid)
{
    m_MediaBuffer.setReferenced(valid);
}

void SystemMemoryVideoBuffer::waitUntilFree(void)
{
    m_MediaBuffer.waitUntilFree();
}

void SystemMemoryVideoBuffer::setWidth(const int width)
{
    m_Width = width;
}

int SystemMemoryVideoBuffer::getWidth(void) const
{
    return m_Width;
}

void SystemMemoryVideoBuffer::setHeight(const int height)
{
    m_Height = height;
}


 int SystemMemoryVideoBuffer::getHeight(void) const
 {
     return m_Height;
 }

MediaSubType SystemMemoryVideoBuffer::getVideoFormat(void) const
{
    return m_PixelFormat;
}

const PlaneInfo& SystemMemoryVideoBuffer::lockPlane(void)
{
    m_MediaBuffer.lock();
    return m_PlaneInfo;
}

void SystemMemoryVideoBuffer::unlockPlane(void)
{
    m_MediaBuffer.unlock();
}

void SystemMemoryVideoBuffer::setTimestamp(micro_seconds timestamp)
{
    m_MediaBuffer.setTimestamp(timestamp);
}

micro_seconds SystemMemoryVideoBuffer::getTimestamp(void) const
{
    return m_MediaBuffer.getTimestamp();
}

void* SystemMemoryVideoBuffer::getPrivateData(void)
{
    return nullptr;
}

Attributes*  SystemMemoryVideoBuffer::getAttributes(void)
{
    return nullptr;
}

//=================================================================================

void SystemMemoryVideoBuffer::allocate(void)
{    
    PixelFormatDescription pfDesc = PixelFormatDescriptor::getPixelFormatDescription(m_PixelFormat);

    uint32 allocateSize = 0;   

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

    m_MediaBuffer.allocate(allocateSize);

    m_PlaneInfo.setPixelBufferPtr(m_MediaBuffer.getBufferPtr());
}

void SystemMemoryVideoBuffer::deallocate(void)
{
    m_MediaBuffer.deallocate();

    m_PlaneInfo.clearPlaneDatas();
}

} // namespace mf
} // namespace surveon
