#ifndef SMF_WRAPPER_MEDIA_BUFFER_H
#define SMF_WRAPPER_MEDIA_BUFFER_H

#include "framework/SMFPrerequisites.h"
#include "framework/thread/SMFMutex.h"
#include "framework/util/SMFReferenceCount.h"
#include "SMFIMediaBuffer.h"


namespace surveon
{
namespace mf
{

class SMF_API WrapperMediaBuffer : public IMediaBuffer
{
public:

    WrapperMediaBuffer(void);
    virtual ~WrapperMediaBuffer(void);

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

    virtual Attributes* getAttributes(void);


    void setMaxLength(size_t length)
    {
        m_MaxLength = length;
    }

    void setBufferPtr(uint8* buffer)
    {
        m_pBuffer = buffer;
    }

    uint8* getBufferPtr(void)
    {
        return m_pBuffer;
    }


protected:
    uint8* m_pBuffer;

    SMF_ATOMIC <size_t> m_MaxLength;
    SMF_ATOMIC<size_t> m_CurrentLength;
    SMF_ATOMIC<bool> m_Valid;
    SMF_ATOMIC<bool> m_Locked;
    SMF_ATOMIC<micro_seconds> m_Timestamp;

    //RecursiveMutex m_Mutex;
    //ConditionVariable m_ConditionVariable;

    ReferenceCount m_ReferenceCount;
};

} // namespace mf
} // namespace surveon

#endif // SMF_WRAPPER_MEDIA_BUFFER_H
