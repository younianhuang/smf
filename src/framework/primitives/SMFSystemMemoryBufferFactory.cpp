

#include "SMFSystemMemoryBufferFactory.h"


namespace surveon
{
namespace mf
{

static SystemMemoryMediaBufferFactory s_SystemMemoryMediaBufferFactory;


IMediaBufferFactory* getSystemMemoryMediaBufferFactory(void)
{
    return &s_SystemMemoryMediaBufferFactory;
}

//====================================================================================

IMediaBuffer* SystemMemoryMediaBufferFactory::createMediaBuffer(size_t size, uint8 aligment)
{
    return m_MediaBufferFactory.create<SystemMemoryMediaBuffer>(size, aligment);
}

void SystemMemoryMediaBufferFactory::destroyMediaBuffer(IMediaBuffer* pBuffer)
{
    m_MediaBufferFactory.destroy(pBuffer);
}

IVideoBuffer* SystemMemoryMediaBufferFactory::createVideoBuffer(uint16 width, uint16 height, MediaSubType format, uint8 aligment)
{
    if(width == 0 || height == 0)
    {
        SMF_THROW_EXCEPT(InvalidParametersException,
                         "Invalid size: width = " << width << " height = " << height);
    }

    if(format != SMF_VIDEO_FORMAT_RGBA && format != SMF_VIDEO_FORMAT_YUV420 &&
       format != SMF_VIDEO_FORMAT_YV12 )
    {
        SMF_THROW_EXCEPT(InvalidParametersException, "Invalid format " << format);
    }

    return m_VideoBufferFactory.create<SystemMemoryVideoBuffer>
            (width, height, format, aligment);
}

void SystemMemoryMediaBufferFactory::destroyVideoBuffer(IVideoBuffer* pBuffer)
{
   m_VideoBufferFactory.destroy(pBuffer);
}

} // namespace mf
} // namespace surveon



