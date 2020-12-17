#include "SMFffmpegPlugin.h"
#include "SMFAVISourceFactory.h"
#include "SMFffmpegDecoderFactory.h"
#include "SMFffmpegManager.h"
//#include "SMFFFmpegHelper.h"

namespace surveon
{
namespace mf
{

ffmpegPlugin::ffmpegPlugin(MediaManager* pMediaManager) :
    m_Name("ffmpeg"),
    m_pMediaManager(pMediaManager)
{
    assert(pMediaManager);
}

ffmpegPlugin::~ffmpegPlugin()
{

}

const String& ffmpegPlugin::getName(void) const
{
    return m_Name;
}

void ffmpegPlugin::install(void)
{    
    m_pAVISourceFactory.reset(new AVISourceFactory("avi"));
    m_pMediaManager->registerFileSourceFactory(m_pAVISourceFactory.get());

    //m_pMP4SourceFactory.reset(new AVISourceFactory("mp4"));
    //m_pMediaManager->registerFileSourceFactory(m_pMP4SourceFactory.get());

    m_pMediaTransformFactory.reset(new ffmpegDecoderFactory());

    m_pMediaManager->registerMediaTransformFactory(m_pMediaTransformFactory.get());
}

void ffmpegPlugin::uninstall(void)
{    
    m_pMediaManager->unregisterFileSourceFactory(m_pAVISourceFactory.get());
    m_pAVISourceFactory.reset();

    m_pMediaManager->unregisterMediaTransformFactory(m_pMediaTransformFactory.get());
    m_pMediaTransformFactory.reset();
}

void ffmpegPlugin::initialize(void)
{
    ffmpegLogManager::getInstancePtr()->initialize(this->getName(), "ffmpeg.log");

    SMF_FFMPEG_LOG_INFO( "Plugin " << this->getName()  << " for SMF "
                     <<  SMF_VERSION_MAJOR <<"."
                     << SMF_VERSION_MINOR << "."
                     << SMF_VERSION_PATCH << ".");

    SMF_FFMPEG_LOG_INFO( "Initialize Plugin");

    ffmpegManager::getInstancePtr()->initialize();
}

void ffmpegPlugin::shutdown(void)
{
    SMF_FFMPEG_LOG_INFO( "Shutdown Plugin");

    ffmpegManager::getInstancePtr()->shutdown();
}



} // namespace mf
} // namespace surveon



