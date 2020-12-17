
#ifndef SMF_FFMPEG_MANAGER_H
#define SMF_FFMPEG_MANAGER_H

#include "SMFffmpegPrerequisites.h"
#include "SMFffmpegLog.h"
#include "SMFHWAcceleration.h"
#include "SMFHWAccelerationDevice.h"
#include "SMFFFmpegHelper.h"

namespace surveon
{
namespace mf
{


/**
 */
class ffmpegManager : public mf::Singleton<ffmpegManager, Mutex>
{
    friend class Singleton<ffmpegManager, Mutex>;

public:
    struct Capability
    {
        bool hardwareAcceleration;

        Capability(): hardwareAcceleration(true)
        {

        }
    };

public:

    void initialize(void);
    void shutdown(void);

    HWAcceleration* createHWAccel(AVCodecContext* pContext, IDeviceManager* pDeviceManager);
    void destroyHWAccel(AVCodecContext* pContext);


private:
    ffmpegManager();
    ~ffmpegManager();

    void createHWAccelerationDevice(IDeviceManager* pDeviceManager);

private:
    Capability m_Capability;

    std::unique_ptr<HWAccelerationDevice> m_pHWAccelDevice;
};


} // namespace mf
} // namespace surveon



#endif // SMF_FFMPEG_MANAGER_H
