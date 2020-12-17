/*

*/
#include "GLVRVideoRendererManager.h"
#include "GLVRVideoRendererManagerImpl.h"
#include <framework/SMFThread.h>


namespace surveon
{
namespace glvr
{

static mf::Mutex g_Mutex;

VideoRendererManager* VideoRendererManager::ms_pInstance = nullptr;

//============================================================================

VideoRendererManager& VideoRendererManager::getInstance(void)
{
    return *getInstancePtr();
}

VideoRendererManager* VideoRendererManager::getInstancePtr(void)
{
    SMF_MUTEX_LOCK(g_Mutex);
    if(ms_pInstance == nullptr)
    {
        ms_pInstance = new VideoRendererManager();
    }

    return ms_pInstance;
}

void VideoRendererManager::destroyInstance(void)
{
    SMF_MUTEX_LOCK (g_Mutex);
    if(ms_pInstance != nullptr)
    {
        delete ms_pInstance;
        ms_pInstance = nullptr;
    }
}

//============================================================================

VideoRendererManager::VideoRendererManager() :
    m_pImpl(new VideoRendererManagerImpl())
{

}

VideoRendererManager::~VideoRendererManager()
{
    delete m_pImpl;
}

void VideoRendererManager::initialize(const String& logFilename)
{
    m_pImpl->initialize(logFilename);
}

void VideoRendererManager::shutdown(void)
{
    m_pImpl->shutdown();
}

IVideoRenderer* VideoRendererManager::createVideoRenderer()
{
    return m_pImpl->createVideoRenderer();
}

void VideoRendererManager::destroyVideoRenderer(IVideoRenderer* pVideoRenderer)
{
    m_pImpl->destroyVideoRenderer(pVideoRenderer);
}

void VideoRendererManager::destroyAllVideoRenderers(void)
{
    m_pImpl->destroyAllVideoRenderers();
}

ITexture* VideoRendererManager::createTexture()
{
    return m_pImpl->createTexture();
}

void VideoRendererManager::destroyTexture(ITexture* pTexture)
{
    m_pImpl->destroyTexture(pTexture);
}

void VideoRendererManager::destroyAllTextures(void)
{
    m_pImpl->destroyAllTextures();
}

Capabilities VideoRendererManager::getCapabilities(void) const
{
    return m_pImpl->getCapabilities();
}

/*
Font* VideoRendererManager::createFont()
{
    return m_pImpl->createFont();
}

void VideoRendererManager::destroyFont(Font* pFont)
{
    m_pImpl->destroyFont(pFont);
}

Shader* VideoRendererManager::createShader()
{
    return m_pImpl->createShader();
}

void VideoRendererManager::destroyShader(Shader* pShader)
{
    m_pImpl->destroyShader(pShader);
}
*/


} // namespace glvr
} // namespace surveon

