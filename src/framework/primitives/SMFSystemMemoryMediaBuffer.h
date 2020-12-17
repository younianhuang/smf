#ifndef SMF_SYSTEM_MEMORY_MEDIA_BUFFER_H
#define SMF_SYSTEM_MEMORY_MEDIA_BUFFER_H

#include "framework/SMFPrerequisites.h"
#include "framework/thread/SMFMutex.h"
#include "framework/primitives/SMFIMediaBuffer.h"
#include "framework/util/SMFReferenceCount.h"


namespace surveon
{
namespace mf
{


class SystemMemoryMediaBuffer : public IMediaBuffer
{
public:

    SystemMemoryMediaBuffer(void);
    SystemMemoryMediaBuffer(size_t size, uint8 aligment=1);
    virtual ~SystemMemoryMediaBuffer(void);        

    virtual size_t getMaxLength(void) const;

    virtual void setCurrentLength(size_t length);
    virtual size_t getCurrentLength(void) const;

    virtual uint8* lock(void);
    virtual void unlock(void);
    virtual bool isLocked(void) const;

    virtual bool isReferenced(void) const;
    virtual void setReferenced(bool valid);
    virtual void waitUntilFree(void);

    virtual void setTimestamp(micro_seconds timestamp);
    virtual micro_seconds getTimestamp(void) const;

    virtual void* getPrivateData(void);

    Attributes* getAttributes(void);

    void allocate(size_t size, uint8 aligment=1);
    void deallocate(void);

    uint8* getBufferPtr(void)
    {
        return m_pBuffer.get();
    }

protected:
    std::unique_ptr<uint8> m_pBuffer;

    SMF_ATOMIC <size_t> m_MaxLength;
    SMF_ATOMIC<size_t> m_CurrentLength;
    SMF_ATOMIC<bool> m_Referenced;
    SMF_ATOMIC<bool> m_Locked;
    SMF_ATOMIC<micro_seconds> m_Timestamp;

    ReferenceCount m_ReferenceCount;
};

} // namespace mf
} // namespace surveon

#endif // SMF_MEDIA_BUFFER_H
