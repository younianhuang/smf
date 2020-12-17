#include "SMFLIVE555Plugin.h"



namespace surveon
{
namespace mf
{

LIVE555Plugin::LIVE555Plugin(MediaManager* pMediaManager) :
    m_Name("LIVE555"),
    m_pMediaManager(pMediaManager)
{
    assert(pMediaManager);
}

LIVE555Plugin::~LIVE555Plugin()
{

}

const String& LIVE555Plugin::getName(void) const
{
    return m_Name;
}

void LIVE555Plugin::install(void)
{
    m_pRTSPSourceFactory.reset(new RTSPSourceFactory());

    m_pMediaManager->registerURLSourceFactory(m_pRTSPSourceFactory.get());

}

void LIVE555Plugin::uninstall(void)
{
    m_pMediaManager->unregisterURLSourceFactory(m_pRTSPSourceFactory.get());

    m_pRTSPSourceFactory.reset();
}

void LIVE555Plugin::initialize(void)
{

}

void LIVE555Plugin::shutdown(void)
{

}



} // namespace mf
} // namespace surveon




