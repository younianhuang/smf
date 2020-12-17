
#ifndef SMF_MEDIA_MANAGER_H
#define SMF_MEDIA_MANAGER_H

#include "framework/SMFPrerequisites.h"
#include "framework/SMFUtil.h"
#include "framework/SMFThread.h"
#include "framework/pipeline/SMFIMediaSourceFactory.h"
#include "framework/pipeline/SMFIMediaTransformFactory.h"
#include "framework/pipeline/SMFIPlugin.h"
#include "framework/pipeline/SMFIPresentationClock.h"


namespace surveon
{
namespace mf
{

class PluginManager;
class SourceResolver;
class MediaTransformManager;

/**
 */
class SMF_API MediaManager : public mf::Singleton<MediaManager>
{
    friend class Singleton<MediaManager>;

public:

    void initialize(const String& logFilename = "smf.log");
    void shutdown(void);

    void loadPlugin(const String& pluginName);
    void unloadPlugin(const String& pluginName);
    void unloadAllPlugins(void);
    void installPlugin(IPlugin* pPlugin);
    void uninstallPlugin(IPlugin* pPlugin);

    IMediaSource* createSourceFromFile(const String& filename);
    void destroyFileSource(IMediaSource* pSource);

    void registerFileSourceFactory(IFileSourceFactory* pFactory);
    void unregisterFileSourceFactory(IFileSourceFactory* pFactory);

    IMediaSource* createSourceFromURL(const String& url);
    void destroyURLSource(IMediaSource* pSource);

    void destroyMediaSource(IMediaSource* pSource);

    void registerURLSourceFactory(IURLSourceFactory* pFactory);
    void unregisterURLSourceFactory(IURLSourceFactory* pFactory);


    IMediaTransform* createMediaTransform(const String& typeId);
    void destroyMediaTransform(IMediaTransform* pTansform);
    Attributes* getMediaTransformAttributes(const String& typeId);

    void registerMediaTransformFactory(IMediaTransformFactory* pFactory);
    void unregisterMediaTransformFactory(IMediaTransformFactory* pFactory);

    IClock* createClock(ClockType type);
    void destroyClock(IClock* pClock);
    IPresentationClock* createPresentationClock(void);
    void destroyPresentationClock(IPresentationClock* pPresentationClock);

    AsyncCommandQueue* getCommandQueue(void);

private:
    MediaManager();
    ~MediaManager();

private:
    bool m_Initialized;

    std::unique_ptr<SourceResolver> m_pSourceResolver;

    std::unique_ptr<MediaTransformManager> m_pMediaTransformManager;

    std::unique_ptr<PluginManager> m_pPluginManager;

    HierarchyObjectFactory<IClock> m_ClockFactory;

    HierarchyObjectFactory<IPresentationClock> m_PresentationClockFactory;

    /// General work queue for async command
    WorkQueue m_GeneralWorkQueue;

    mutable Mutex m_Mutex;
};

} // namespace mf
} // namespace surveon



#endif // SMF_MEDIA_MANAGER_H




