
#ifndef SMF_HW_ACCELERATION_H
#define SMF_HW_ACCELERATION_H


#include "SMFffmpegPrerequisites.h"
#include "SMFFFmpegHelper.h"


namespace surveon
{
namespace mf
{


class HWAcceleration
{
public:

    virtual ~HWAcceleration(void)  {}

    virtual void initialize(void) = 0;

    virtual void shutdown(void) = 0;

    virtual void retriveVideoData(AVFrame* pFrame, IVideoBuffer* pVideoBuffer) = 0;

    virtual void setDirectVideoOutTarget(mf::WindowHandle handle) = 0;

    virtual MediaSubType getFormat(void) const = 0;
};


} // namespace mf
} // namespace surveon


#endif // SMF_HW_ACCELERATION_H


