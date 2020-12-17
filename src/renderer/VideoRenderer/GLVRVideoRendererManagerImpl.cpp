#include "GLVRVideoRendererManagerImpl.h"
#include <framework/SMFLog.h>


namespace surveon
{
namespace glvr
{


VideoRendererManagerImpl::VideoRendererManagerImpl() :
    m_CommandQueue(&m_CommandSyncControl),
    m_Initialize(false),
    m_Run(false)
{

}

VideoRendererManagerImpl::~VideoRendererManagerImpl()
{
    shutdown();
}

void VideoRendererManagerImpl::initialize(const String& logFilename)
{    
    SMF_MUTEX_LOCK(m_Mutex);

    if(m_Initialize)
        return;

    mf::log::LogManager::getInstancePtr()->initialize();

    if(!logFilename.empty())
        glvr::Log::setup("glvr", logFilename);

    GLVR_LOG_INFO("Initialize library OpenGL Video Renderer "
                  << GLVR_VERSION_MAJOR <<"."
                  << GLVR_VERSION_MINOR << "."
                  << GLVR_VERSION_PATCH << "."
                  );

    m_Run = true;

    //NOTE:
    //Primary window must be created and destroyed in GUI thread.
    //So call initialize here
    //RenderEngine::getInstance().initialize();
    //m_Initialize = true;

    m_Thread = Thread(&VideoRendererManagerImpl::run, this);

    m_CommandQueue.blockingCall<void>(
                        AsyncCommand::CT_CREATE,
                        &VideoRendererManagerImpl::initializeCommand,
                        this );

    GLVR_LOG_INFO("library OpenGL Video Renderer is initialized.");
}

void VideoRendererManagerImpl::shutdown(void)
{
    SMF_MUTEX_LOCK(m_Mutex);
    if(m_Initialize)
    {
        GLVR_LOG_INFO("Shutdown library OpenGL Video Renderer.");

        m_Run = false;

        m_Thread.join();        

        m_Initialize = false;

        GLVR_LOG_INFO("Library OpenGL Video Renderer is shutdown.");
    }
}

IVideoRenderer* VideoRendererManagerImpl::createVideoRenderer()
{        
    return m_CommandQueue.blockingCall<IVideoRenderer*>(
                                    AsyncCommand::CT_CREATE,
                                    &VideoRendererManagerImpl::createVideoRendererCommand, this);
}

void VideoRendererManagerImpl::destroyVideoRenderer(IVideoRenderer* pRender)
{
    m_CommandQueue.blockingCall<void>(
                                        AsyncCommand::CT_DESTROY,
                                        &VideoRendererManagerImpl::destroyVideoRendererCommand, this
                                        , pRender);
}

void VideoRendererManagerImpl::destroyAllVideoRenderers(void)
{
    //NOTE:
    // IVideoRenderer::shutdown must be called in GUI thread.
    // So call IVideoRenderer::shutdown here
    for(auto  pRender : m_VideoRenderSet)
    {
        pRender->shutdown();
    }

    m_CommandQueue.blockingCall<void>(
                                        AsyncCommand::CT_DESTROY,
                                        &VideoRendererManagerImpl::destroyAllVideoRendererCommand, this);
}

ITexture* VideoRendererManagerImpl::createTexture(void)
{
    return m_CommandQueue.blockingCall<ITexture*>(
                    AsyncCommand::CT_CREATE,
                    &VideoRendererManagerImpl::createTextureCommand,
                    this );
}

void VideoRendererManagerImpl::destroyTexture(ITexture* pTexture)
{
    m_CommandQueue.blockingCall<void>(
                    AsyncCommand::CT_DESTROY,
                    &TextureFactory::destroy,
                    &m_TextureFactory,
                    pTexture );
}

void VideoRendererManagerImpl::destroyAllTextures(void)
{
    m_CommandQueue.blockingCall<void>(
                    AsyncCommand::CT_DESTROY,
                    &TextureFactory::destroyAll,
                    &m_TextureFactory);
}
Capabilities VideoRendererManagerImpl::getCapabilities(void) const
{
    return m_CommandQueue.blockingCall<Capabilities>(
                    AsyncCommand::CT_GET,
                    &RenderEngine::getCapabilities,
                    &RenderEngine::getInstance());
}

/*
GLInfo VideoRendererManagerImpl::getGLInfo(void)
{
    return m_CommandQueue.blockingCall<GLInfo>(
                    AsyncCommand::CT_GET,
                    &RenderEngine::getCapabilities,
                    &RenderEngine::getInstance());
}
*/

/*
Font* VideoRendererManagerImpl::createFont()
{
    return 0;
}

void VideoRendererManagerImpl::destroyFont(Font*)
{

}

Shader* VideoRendererManagerImpl::createShader()
{
    return 0;
}

void VideoRendererManagerImpl::destroyShader(Shader*)
{

}

*/
void VideoRendererManagerImpl::run(void)
{
    try
    {
        while(m_Run)
        {
            m_CommandQueue.executeCommands();

            renderOneFrame();

            waitForCommand();
        }

        releaseResourceCommand();
    }
    catch(std::exception& ex)
    {
        GLVR_LOG_ERROR("Rendering Thread stopped caused by exception: " << ex.what());
    }
    catch(...)
    {
        GLVR_LOG_ERROR("Rendering Thread stopped caused by unknown exception ") ;
    }
}

//================================================================================================================================
void VideoRendererManagerImpl::initializeCommand(void)
{
    try
    {
        RenderEngine::getInstance().initialize();

        m_Initialize = true;
    }
    catch(...)
    {
        m_Run = false;
        throw;
    }

}

 void VideoRendererManagerImpl::releaseResourceCommand(void)
 {
     destroyAllVideoRendererCommand();

     m_TextureFactory.destroyAll();

     RenderEngine::getInstance().shutdown();
 }


IVideoRenderer* VideoRendererManagerImpl::createVideoRendererCommand()
{   
    VideoRendererImpl* pRender = new VideoRendererImpl(&m_CommandSyncControl);

    m_VideoRenderSet.insert(pRender);

    return pRender;        
}

void VideoRendererManagerImpl::destroyVideoRendererCommand(IVideoRenderer* pRender)
{
    if(!mf::ContainerUtil::destroySetElement(m_VideoRenderSet, static_cast<VideoRendererImpl*>(pRender)))
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Specified pointer does not exist!");
    }
}

void VideoRendererManagerImpl::destroyAllVideoRendererCommand(void)
{
    mf::ContainerUtil::destroyAllElements(m_VideoRenderSet);
}

ITexture* VideoRendererManagerImpl::createTextureCommand(void)
{
    return m_TextureFactory.create(&m_CommandQueue);
}

void VideoRendererManagerImpl::renderOneFrame(void)
{
    for(auto pRender : m_VideoRenderSet)
    {
        pRender->executeCommands();

        if(pRender->isInitialized())
        {
            pRender->render();
        }
    }
}

void VideoRendererManagerImpl::waitForCommand(void)
{
    if(m_CommandQueue.hasMoreCommand() || !m_Run)
        return;

    micro_seconds delay = 10000;

    for(auto pRender : m_VideoRenderSet)
    {
        delay = std::min(delay, pRender->predictDelayTime());
    }

    if(delay > 0)
    {
        m_CommandSyncControl.waitCommandForMicroSeconds(delay);
    }
}

} // namespace glvr
} // namespace surveon
