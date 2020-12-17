#ifndef GLVR_PIXMAP_VIDEOBUFFER_H
#define GLVR_PIXMAP_VIDEOBUFFER_H

#include "GLVRImplPrerequisites.h"
#include "GLVRPixelBuffer.h"


namespace surveon
{
namespace glvr
{


class HardwareVideoBuffer : public IVideoBuffer
{
public:
    HardwareVideoBuffer(HardwarePixelBufferSharedPtr pPixmap);

    virtual ~HardwareVideoBuffer(void);

    virtual size_t getMaxLength(void) const;

    virtual void setCurrentLength(size_t length);
    virtual size_t getCurrentLength(void) const;

    virtual uint8* lock(void);
    virtual void unlock(void);
    virtual bool isLocked(void) const;

    virtual bool isReferenced(void) const;
    virtual void setReferenced(bool valid);
    virtual void waitUntilFree(void);

    virtual void setWidth(const int width);
    virtual int getWidth(void) const;

    virtual void setHeight(const int height);
    virtual int getHeight(void) const;

    virtual MediaSubType getVideoFormat(void) const;
    virtual const PlaneInfo& lockPlane(void);
    virtual void unlockPlane(void);

    virtual void setTimestamp(micro_seconds timestamp);
    virtual micro_seconds getTimestamp(void) const;

    virtual void* getPrivateData(void);

    Attributes* getAttributes(void);

    HardwarePixelBufferSharedPtr getPixelBuffer(void)
    {
        return m_pHardwarePixelBuffer;
    }


private:

    size_t m_MaxLength;
    SMF_ATOMIC<bool> m_Referenced;
    SMF_ATOMIC<bool> m_Locked;
    SMF_ATOMIC<micro_seconds> m_Timestamp;

    PlaneInfo m_PlaneInfo;

    HardwarePixelBufferSharedPtr m_pHardwarePixelBuffer;

    ReferenceCount m_ReferenceCount;
};

} // namespace mf
} // namespace surveon

#endif // GLVR_PIXMAP_VIDEOBUFFER_H


