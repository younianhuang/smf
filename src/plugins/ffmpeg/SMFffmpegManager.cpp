
#include "SMFffmpegManager.h"
#include "SMFFFmpegHelper.h"

#ifdef SMF_PLATFORM_LINUX
//#include "vdpau/SMFVDPAUDevice.h"
#include "vaapi/SMFVAAPIDevice.h"
#else
#include "dxva2/SMFDXVA2Device.h"
#endif


namespace surveon
{
namespace mf
{

ffmpegManager::ffmpegManager()
{

}

ffmpegManager::~ffmpegManager()
{

}

void ffmpegManager::initialize(void)
{
    SMF_FFMPEG_LOG_INFO("Initialize ffmpeg");

    ffmpegHelper::initialize();

    try
    {
        //createHWAccelerationDevice();

        //m_Capability.hardwareAcceleration = true;
    }
    catch(...)
    {

    }
}

void ffmpegManager::shutdown(void)
{
    if(m_pHWAccelDevice)
    {        
        m_pHWAccelDevice.reset();
    }

    ffmpegHelper::shutdown();
}

void ffmpegManager::createHWAccelerationDevice(IDeviceManager* pDeviceManager)
{
    try
    {
        #ifdef SMF_PLATFORM_LINUX
                m_pHWAccelDevice.reset(new VAAPIDevice);
        #else
                m_pHWAccelDevice.reset(new DXVA2Device(pDeviceManager));
        #endif

        m_pHWAccelDevice->initialize();
    }
    catch(...)
    {
        m_Capability.hardwareAcceleration = false;
    }
}

HWAcceleration* ffmpegManager::createHWAccel(AVCodecContext* pContext, IDeviceManager* pDeviceManager)
{    
    if(!m_pHWAccelDevice && m_Capability.hardwareAcceleration)
    {
            createHWAccelerationDevice(pDeviceManager);
    }

    if(m_pHWAccelDevice)
    {
        return m_pHWAccelDevice->createHWAccel(pContext);
    }

    return nullptr;
}

void ffmpegManager::destroyHWAccel(AVCodecContext* pContext)
{
    if(m_pHWAccelDevice)
        m_pHWAccelDevice->destroyHWAccel(pContext);
}


} // namespace mf
} // namespace surveon


