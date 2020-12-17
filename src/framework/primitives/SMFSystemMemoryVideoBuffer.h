#ifndef SMF_SYSTEM_MEMORY_VIDEOBUFFER_H
#define SMF_SYSTEM_MEMORY_VIDEOBUFFER_H

#include "SMFSystemMemoryMediaBuffer.h"
#include "framework/primitives/SMFIVideoBuffer.h"

namespace surveon
{
namespace mf
{


class SystemMemoryVideoBuffer : public IVideoBuffer
{
public:
    SystemMemoryVideoBuffer(int width, int height, MediaSubType format, uint8 aligment = 1);
    virtual ~SystemMemoryVideoBuffer(void);

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

private:
    void allocate(void);
    void deallocate(void);

private:
    SystemMemoryMediaBuffer m_MediaBuffer;
    PlaneInfo m_PlaneInfo;
    SMF_ATOMIC<int> m_Width;
    SMF_ATOMIC<int> m_Height;
    MediaSubType m_PixelFormat;
};

} // namespace mf
} // namespace surveon

#endif // SMF_SYSTEM_MEMORY_VIDEOBUFFER_H
