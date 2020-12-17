#ifndef SMF_WRAPPER_VIDEOBUFFER_H
#define SMF_WRAPPER_VIDEOBUFFER_H

#include "framework/SMFPrerequisites.h"
#include "framework/thread/SMFMutex.h"
#include "SMFWrapperMediaBuffer.h"
#include "SMFIVideoBuffer.h"

namespace surveon
{
namespace mf
{


class SMF_API WrapperVideoBuffer : public IVideoBuffer
{
public:
    WrapperVideoBuffer(void);
    WrapperVideoBuffer(int width, int height, MediaSubType format);
    virtual ~WrapperVideoBuffer(void);

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

    virtual void setPixelFormat(MediaSubType format);
    virtual MediaSubType getVideoFormat(void) const;
    virtual const PlaneInfo& lockPlane(void);
    virtual void unlockPlane(void);

    virtual void setMaxLength(size_t length)
    {
        m_MediaBuffer.setMaxLength(length);
    }

    virtual void setTimestamp(micro_seconds timestamp);
    virtual micro_seconds getTimestamp(void) const;

    virtual void* getPrivateData(void);

    Attributes* getAttributes(void);

    void setBufferPtr(uint8* buffer);

    uint8* getBufferPtr(void)
    {
        return m_MediaBuffer.getBufferPtr();
    }

private:
    WrapperMediaBuffer m_MediaBuffer;
    PlaneInfo m_PlaneInfo;
    SMF_ATOMIC<int> m_Width;
    SMF_ATOMIC<int> m_Height;
    MediaSubType m_PixelFormat;
};

} // namespace mf
} // namespace surveon

#endif // SMF_WRAPPER_VIDEOBUFFER_H
