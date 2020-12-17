
#ifndef SMF_IMEDIA_BUFFER_H
#define SMF_IMEDIA_BUFFER_H

#include "framework/SMFPrerequisites.h"
#include "framework/primitives/SMFAttributes.h"

namespace surveon
{
namespace mf
{

#define SMF_ALIGN(x, a) (((x)+(a)-1)&~((a)-1))


/** Class that manages a block of memory, typically to hold media data.
 * Media buffers are used to move data from one pipeline component to the next.
 */
class SMF_API IMediaBuffer
{
public:
    virtual ~IMediaBuffer(void) {}

    /** Gets the allocated size of the buffer.*/
    virtual size_t getMaxLength(void) const = 0;

    /** Sets the length of the valid data in the buffer. */
    virtual void setCurrentLength(size_t length) = 0;

    /** Gets the length of the valid data in the buffer. */
    virtual size_t getCurrentLength(void) const = 0;

    /** Gives the caller access to the memory in the buffer, for reading or writing.
     * @remarks
     *      Locking the buffer does not prevent other threads from calling Lock,
     *      so you should not rely on this method to synchronize threads.
    */
    virtual uint8* lock(void) = 0;

    /** Unlocks a buffer that was previously locked.
    */
    virtual void unlock(void) = 0;

    /** Gets whether this buffer is locked. */
    virtual bool isLocked(void) const = 0;

    /** Gets whether this buffer is used. */
    virtual bool isReferenced(void) const = 0;

    /** Sets whether this buffer is in use. */
    virtual void setReferenced(bool valid) = 0;

    /** Wait until this buffer is not used.
     * @remarks
     *    This blocks the caller thread until this buffer content become invalid.
    */
    virtual void waitUntilFree(void) = 0;

    virtual void setTimestamp(micro_seconds timestamp) = 0;

    virtual micro_seconds getTimestamp(void) const = 0;

    virtual void* getPrivateData(void) = 0;

    virtual Attributes* getAttributes(void) = 0;

private:

};

} // namespace mf
} // namespace surveon

#endif // SMF_IMEDIA_BUFFER_H

