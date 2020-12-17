#include "SMFRTSPSourceFactory.h"

#include "SMFRTSPSourceFactory.h"

namespace surveon
{
namespace mf
{

/**
 */
RTSPSourceFactory::RTSPSourceFactory() :
    m_Scheme("rtsp")
{

}

RTSPSourceFactory::~RTSPSourceFactory()
{

}


IMediaSource* RTSPSourceFactory::create(const String& url)
{
    IMediaSource* pSource = m_FactoryImpl.create();
    pSource->initialize(url);
    return pSource;
}

 void RTSPSourceFactory::destroy(IMediaSource* pSource)
 {
    m_FactoryImpl.destroy(pSource);
 }

const String& RTSPSourceFactory::getScheme(void) const
{
    return m_Scheme;
}



} // namespace mf
} // namespace surveon





