#ifndef SMF_IVIDEO_BUFFER_H
#define SMF_IVIDEO_BUFFER_H


#include "framework/primitives/SMFIMediaBuffer.h"
#include "framework/primitives/SMFPlaneInfo.h"
#include "framework/primitives/SMFMediaType.h"

namespace surveon
{
namespace mf
{

/** Class that manages video frame data.
 *    VideoBuffer is a 2D a two-dimensional memory with pixel format.
 */
class SMF_API IVideoBuffer : public IMediaBuffer
{
public:
    virtual ~IVideoBuffer(void) {}

    /** Sets width of this buffer. */
    virtual void setWidth(const int width) = 0;

    /** Gets width of this buffer. */
    virtual int getWidth(void) const = 0;

    /** Sets height of this buffer. */
    virtual void setHeight(const int height) = 0 ;

    /** Gets height of this buffer. */
    virtual int getHeight(void) const = 0;

    /** Gets pixel format of this buffer. */
    virtual MediaSubType getVideoFormat(void) const = 0;

    /** Gives the caller access to the plane in the buffer, for reading or writing.
     * @remarks
     *      Locking the buffer does not prevent other threads from calling Lock,
     *      so you should not rely on this method to synchronize threads.
    */
    virtual const PlaneInfo& lockPlane(void) = 0;

    /** Unlocks a buffer that was previously locked.
    */
    virtual void unlockPlane(void) = 0;

private:

};

} // namespace mf
} // namespace surveon



#endif // SMF_IVIDEO_BUFFER_H

