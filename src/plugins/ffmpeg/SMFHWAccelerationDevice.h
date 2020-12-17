
#ifndef SMF_HW_ACCELERATION_DEVICE_H
#define SMF_HW_ACCELERATION_DEVICE_H


#include "SMFffmpegPrerequisites.h"
#include "SMFHWAcceleration.h"
#include "SMFFFmpegHelper.h"


namespace surveon
{
namespace mf
{


class HWAccelerationDevice
{
public:

    virtual ~HWAccelerationDevice(void)  {}

    virtual void initialize(void) = 0;

    virtual void shutdown(void) = 0;

    virtual HWAcceleration* createHWAccel(AVCodecContext* pContext) = 0;

    virtual void destroyHWAccel(struct AVCodecContext* pContext) = 0;


};


} // namespace mf
} // namespace surveon


#endif // SMF_HW_ACCELERATION_DEVICE_H


