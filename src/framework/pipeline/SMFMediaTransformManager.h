
#ifndef SMF_MEDIA_TRANSFORM_MANAGER_H
#define SMF_MEDIA_TRANSFORM_MANAGER_H

#include "framework/pipeline/SMFIMediaTransformFactory.h"
#include "SMFCorePrerequisites.h"

namespace surveon
{
namespace mf
{


/**
 */
class MediaTransformManager
{

public:
    MediaTransformManager();
    ~MediaTransformManager();

    void initialize(void);
    void shutdown(void);

    IMediaTransform* createMediaTransform(const String& typeId);
    void destroyMediaTransform(IMediaTransform* pTansform);

    void registerMediaTransformFactory(IMediaTransformFactory* pFactory);
    void unregisterMediaTransformFactory(IMediaTransformFactory* pFactory);

    Attributes* getAttributes(const String& typeId);

private:
    IMediaTransformFactory* getMediaTransformFactory(const String& typeId);

private:
    bool m_Initialized;

    typedef std::map<String, IMediaTransformFactory*> MediaTransformFactoryMap;
    MediaTransformFactoryMap m_MediaTransformFactoryMap;
};

} // namespace mf
} // namespace surveon

#endif // SMF_MEDIA_TRANSFORM_MANAGER_H
