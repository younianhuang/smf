#ifndef GLVR_PIXEL_BUFFER_H
#define GLVR_PIXEL_BUFFER_H


#include "GLVRRenderEnginePrerequisites.h"


namespace surveon
{
namespace glvr
{

class HardwarePixelBuffer;

typedef std::shared_ptr<HardwarePixelBuffer> HardwarePixelBufferSharedPtr;

class HardwarePixelBuffer
{
public:

    virtual ~HardwarePixelBuffer()  {}

    virtual void* getNativeHandle(void) = 0;

    virtual void* getPrivateData(void) = 0;

    virtual void bindToTexture(uint texureId) = 0;

    virtual void unbindToTexture(void) = 0;

    virtual uint32 getWidth(void) const = 0;

    virtual uint32 getHeight(void) const = 0;

    virtual mf::MediaSubType getFormat(void) const = 0;

    virtual void blit(HardwarePixelBufferSharedPtr pSource) = 0;
};


} // namespace glvr
} // namespace surveon


#endif // GLVR_PIXEL_BUFFER_H
