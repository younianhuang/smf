
#include "framework/pipeline/SMFMediaManager.h"
#include "SMFDynamicLibrary.h"
#include "SMFPluginManager.h"
#include "SMFSourceResolver.h"
#include "SMFMediaTransformManager.h"
#include "SMFClockImpl.h"
#include "SMFPresentationClockImpl.h"

namespace surveon
{
namespace mf
{

MediaManager::MediaManager() :
    m_Initialized(false)
{

}

MediaManager::~MediaManager()
{
    shutdown();
}

void MediaManager::initialize(const String& logFilename)
{
    SMF_MUTEX_LOCK(m_Mutex);
    if(m_Initialized)
        return;

    // initialize log
    log::LogManager::getInstancePtr()->initialize();

    CoreLog::getInstancePtr()->initialize(logFilename);

    m_GeneralWorkQueue.initialize();

    SMF_CORE_LOG_INFO("Surveon multi-media framework version "
                 << SMF_VERSION_MAJOR <<"."
                 << SMF_VERSION_MINOR << "."
                 << SMF_VERSION_PATCH << ".");

    SMF_CORE_LOG_INFO("Initialize MediaManager ");

    m_pSourceResolver.reset(new SourceResolver);
    m_pSourceResolver->initialize();

    m_pMediaTransformManager.reset(new MediaTransformManager);
    m_pMediaTransformManager->initialize();

    m_pPluginManager.reset(new PluginManager(this));

    m_pPluginManager->initialize();

    m_Initialized = true;

    SMF_CORE_LOG_INFO("MediaManager is initialized");
}

void MediaManager::shutdown(void)
{
    SMF_MUTEX_LOCK(m_Mutex);
    if(m_Initialized)
    {
        SMF_CORE_LOG_INFO("Shutdown MediaManager");

        m_pPluginManager->shutdown();

        m_pPluginManager.reset();

        m_pMediaTransformManager.reset();

        m_pSourceResolver.reset();

        m_GeneralWorkQueue.shutdown();

        SMF_CORE_LOG_INFO("MediaSource is shutdown");

        log::LogManager::getInstancePtr()->shutdown();
    }
}

void MediaManager::loadPlugin(const String& pluginName)
{
    SMF_MUTEX_LOCK(m_Mutex);
    m_pPluginManager->loadPlugin(pluginName);
}

void MediaManager::unloadPlugin(const String& pluginName)
{
    SMF_MUTEX_LOCK(m_Mutex);
    m_pPluginManager->unloadPlugin(pluginName);
}

void MediaManager::unloadAllPlugins(void)
{    
    SMF_MUTEX_LOCK(m_Mutex);
    m_pPluginManager->shutdown();
}

void MediaManager::installPlugin(IPlugin* pPlugin)
{
    m_pPluginManager->installPlugin(pPlugin);
}

void MediaManager::uninstallPlugin(IPlugin* pPlugin)
{
    m_pPluginManager->uninstallPlugin(pPlugin);
}

void MediaManager::registerFileSourceFactory(IFileSourceFactory* pFactory)
{
    SMF_CORE_LOG_INFO("Register file source: " <<  pFactory->getFileType());

    m_pSourceResolver->registerFileSourceFactory(pFactory);
}

void MediaManager::unregisterFileSourceFactory(IFileSourceFactory* pFactory)
{
    SMF_CORE_LOG_INFO("Unregister file source: " <<  pFactory->getFileType());

    m_pSourceResolver->unregisterFileSourceFactory(pFactory);
}


IMediaSource* MediaManager::createSourceFromFile(const String& filename)
{
    SMF_MUTEX_LOCK(m_Mutex);

    return m_pSourceResolver->createSourceFromFile(filename);
}

void MediaManager::destroyFileSource(IMediaSource* pSource)
{
    SMF_MUTEX_LOCK(m_Mutex);

    m_pSourceResolver->destroyFileSource(pSource);
}

IMediaSource* MediaManager::createSourceFromURL(const String& url)
{
    SMF_MUTEX_LOCK(m_Mutex);

    return m_pSourceResolver->createSourceFromURL(url);
}

void MediaManager::destroyURLSource(IMediaSource* pSource)
{
    SMF_MUTEX_LOCK(m_Mutex);

    m_pSourceResolver->destroyURLSource(pSource);
}

void MediaManager::destroyMediaSource(IMediaSource* pSource)
{
    SMF_MUTEX_LOCK(m_Mutex);

    m_pSourceResolver->destroyMediaSource(pSource);
}

void MediaManager::registerURLSourceFactory(IURLSourceFactory* pFactory)
{
    SMF_CORE_LOG_INFO("Register url source: " <<  pFactory->getScheme());

    m_pSourceResolver->registerURLSourceFactory(pFactory);
}

void MediaManager::unregisterURLSourceFactory(IURLSourceFactory* pFactory)
{
    SMF_CORE_LOG_INFO("Unregister url source: " <<  pFactory->getScheme());

    m_pSourceResolver->unregisterURLSourceFactory(pFactory);
}


IMediaTransform* MediaManager::createMediaTransform(const String& typeId)
{
    SMF_MUTEX_LOCK(m_Mutex);
    return m_pMediaTransformManager->createMediaTransform(typeId);
}

void MediaManager::destroyMediaTransform(IMediaTransform* pTansform)
{
    SMF_MUTEX_LOCK(m_Mutex);
    m_pMediaTransformManager->destroyMediaTransform(pTansform);
}

Attributes* MediaManager::getMediaTransformAttributes(const String& typeId)
{
    SMF_MUTEX_LOCK(m_Mutex);
    return m_pMediaTransformManager->getAttributes(typeId);
}

void MediaManager::registerMediaTransformFactory(IMediaTransformFactory* pFactory)
{
    SMF_CORE_LOG_INFO("Register MediaTransform: " <<  pFactory->getTypeId());

    m_pMediaTransformManager->registerMediaTransformFactory(pFactory);
}

void MediaManager::unregisterMediaTransformFactory(IMediaTransformFactory* pFactory)
{
    SMF_CORE_LOG_INFO("Unregister MediaTransform: " <<  pFactory->getTypeId());

    m_pMediaTransformManager->unregisterMediaTransformFactory(pFactory);
}

IClock* MediaManager::createClock(ClockType type)
{
    switch (type)
    {
    case CT_SYSTEM_CLOCK:
        return m_ClockFactory.create<SystemClock>();

    case CT_HIGH_RESOLUTION_CLOCK:
        return m_ClockFactory.create<HighResolutionClock>();

    default:
        assert(0);
        return nullptr;
    }

}

void MediaManager::destroyClock(IClock* pClock)
{
    m_ClockFactory.destroy(pClock);
}

IPresentationClock* MediaManager::createPresentationClock(void)
{
    return m_PresentationClockFactory.create<PresentationClockImpl>();
}

void MediaManager::destroyPresentationClock(IPresentationClock* pPresentationClock)
{
    m_PresentationClockFactory.destroy(pPresentationClock);
}

AsyncCommandQueue* MediaManager::getCommandQueue(void)
{
    return m_GeneralWorkQueue.getCommandQueue();
}

//===========================================================================




} // namespace mf
} // namespace surveon



