#include "SMFMediaTransformManager.h"

namespace surveon
{
namespace mf
{

MediaTransformManager::MediaTransformManager() :
    m_Initialized(false)
{
    shutdown();
}

MediaTransformManager::~MediaTransformManager()
{

}

void MediaTransformManager::initialize(void)
{
    if(m_Initialized)
        return;

    m_Initialized = true;
}

void MediaTransformManager::shutdown(void)
{
    if(m_Initialized)
    {
        m_MediaTransformFactoryMap.clear();

        m_Initialized = false;
    }
}

void MediaTransformManager::registerMediaTransformFactory(IMediaTransformFactory* pFactory)
{
    assert(pFactory);
    m_MediaTransformFactoryMap[pFactory->getTypeId()] = pFactory;
}

void MediaTransformManager::unregisterMediaTransformFactory(IMediaTransformFactory* pFactory)
{
    assert(pFactory);
    m_MediaTransformFactoryMap.erase(m_MediaTransformFactoryMap.find(pFactory->getTypeId()));
}

IMediaTransform* MediaTransformManager::createMediaTransform(const String& typeId)
{
    IMediaTransformFactory* pFactory=  getMediaTransformFactory(typeId);
    return pFactory->create();
}

void MediaTransformManager::destroyMediaTransform(IMediaTransform* pTansform)
{
    if(pTansform == nullptr)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException, "null pointer is invalid");
    }

    IMediaTransformFactory* pFactory=  getMediaTransformFactory(pTansform->getTypeId());

    pFactory->destroy(pTansform);
}

//===========================================================================

IMediaTransformFactory* MediaTransformManager::getMediaTransformFactory(const String& typeId)
{
    auto it = m_MediaTransformFactoryMap.find(typeId);
    if(it == m_MediaTransformFactoryMap.end())
    {
        SMF_CORE_THROW_EXCEPT(UnsupportFormatException,  "MediaTransform " << typeId << " is not registered");
    }

    return it->second;
}

Attributes* MediaTransformManager::getAttributes(const String& typeId)
{
    IMediaTransformFactory* pFactory =  getMediaTransformFactory(typeId);
    return pFactory->getAttributes();
}



} // namespace mf
} // namespace surveon


