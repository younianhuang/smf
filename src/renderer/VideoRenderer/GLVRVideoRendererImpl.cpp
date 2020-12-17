#include "GLVRVideoRendererImpl.h"
#include "GLVRPixmapVideoBuffer.h"

namespace surveon
{

namespace glvr
{

#define CHECH_INIT_STATE()   \
    if(!m_Initialized) \
    { \
        GLVR_THROW_EXCEPT(GLVRInvalidStateException, "This VideoRenderer is not initialized."); \
    }



int VideoRendererImpl::ms_Count = 0;

VideoRendererImpl::VideoRendererImpl(mf::AsyncCommandQueue::SyncControl* pSyncControl) :
    m_Initialized(false),
    m_Updated(false),
    m_pRenderWindow(nullptr),
    m_CommandQueue(pSyncControl),
    m_WindowHandle(WindowHandle()),
    m_AvgFramerateCounter(5000),
    m_pPresentationClock(nullptr),
    m_Index(ms_Count++)
{
    //GLVR_LOG_INFO( "Video Render 0x" << this << " is created");
    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " is created");  
}

VideoRendererImpl::~VideoRendererImpl(void)
{    
    shutdownCommand();

    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " is destoryed");

    ms_Count--;
}

void VideoRendererImpl::initialize(mf::WindowHandle handle)
{    
    if(m_Initialized)
    {
        GLVR_THROW_EXCEPT(InvalidStateException, "This VideoRenderer is already initialized.");
    }

    GLVR_LOG_INFO("Initialize Video Render " <<  SMF_ADDR_PREFIX(this));

    m_WindowHandle = handle;

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_CREATE,
                &VideoRendererImpl::initializeCommand, this, handle);    
}

void VideoRendererImpl::shutdown(void)
{    
    CHECH_INIT_STATE();

    m_CommandQueue.blockingCall<void>(
            AsyncCommand::CT_DESTROY,
            &VideoRendererImpl::shutdownCommand, this);
}

bool VideoRendererImpl::isInitialized(void) const
{    
    return m_Initialized;
}


void VideoRendererImpl::repaint(void)
{
    CHECH_INIT_STATE();

    m_CommandQueue.asyncCall<void>(
            AsyncCommand::CT_RENDER,
            &VideoRendererImpl::emptyCommand, this);
}

void VideoRendererImpl::setViewport(const mf::FloatRect& rect)
{
    CHECH_INIT_STATE();
    m_CommandQueue.asyncCall<void>(
               AsyncCommand::CT_SET,
               &VideoRendererImpl::setViewportCommand, this,
                rect);
}

IVideoRenderer::Statistics VideoRendererImpl::getStatistics(void) const
{
    CHECH_INIT_STATE();

    return m_CommandQueue.blockingCall<Statistics>(
            AsyncCommand::CT_GET,
            &VideoRendererImpl::getStatisticsCommand, this);
}

void VideoRendererImpl::setPresentationClock(mf::IPresentationClock* pPresentationClock)
{
    CHECH_INIT_STATE();

    m_CommandQueue.asyncCall<void>(
               AsyncCommand::CT_SET,
               &VideoRendererImpl::setPresentationClockCommand, this,
                pPresentationClock);
}

mf::IVideoBuffer* VideoRendererImpl::createGPUHardwareVideoBuffer(uint16 width, uint16 height)
{
    CHECH_INIT_STATE();



    return asyncReturn<mf::IVideoBuffer*>(
                &m_CommandQueue,
                &VideoRendererImpl::createPixmapVideoBufferCommand, this,
                SMF_ALIGN(width, 16),
                SMF_ALIGN(height, 16));
}

void VideoRendererImpl::destroyGPUHardwareVideoBuffer(mf::IVideoBuffer* pVideoBuffer)
{
    CHECH_INIT_STATE();

    asyncReturn<void>(
                    &m_CommandQueue,
                    &VideoRendererImpl::destroyPixmapVideoBufferCommand, this,
                    pVideoBuffer);
}

IDeviceManager* VideoRendererImpl::getDeviceManager(void)
{
    CHECH_INIT_STATE();

    return asyncReturn<IDeviceManager*>(
                    &m_CommandQueue,
                    &VideoRendererImpl::getDeviceManagerCommand, this);
}

IVideoPane* VideoRendererImpl::createVideoPane()
{
    CHECH_INIT_STATE();

    return m_CommandQueue.blockingCall<IVideoPane*>(
            AsyncCommand::CT_CREATE,
            &VideoRendererImpl::createVideoPaneCommand, this);
}

void VideoRendererImpl::destroyVideoPane(IVideoPane* pVideoPane)
{
    CHECH_INIT_STATE();

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &VideoRendererImpl::destroyVideoPaneCommand, this,
                pVideoPane);
}

void VideoRendererImpl::destroyAllVideoPanes(void)
{
    CHECH_INIT_STATE();

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &VideoRendererImpl::destroyAllVideoPanes, this );
}

void VideoRendererImpl::flush(void)
{
    CHECH_INIT_STATE();

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &VideoRendererImpl::flushCommand, this );
}

IRetangleShape* VideoRendererImpl::createRetangleShape(const mf::FloatRect& rect)
{    
    CHECH_INIT_STATE();
    return m_CommandQueue.blockingCall<IRetangleShape*>(
                AsyncCommand::CT_CREATE,
                &VideoRendererImpl::createRetangleShapeCommand, this,
                rect);
}

void VideoRendererImpl::destroyRetangleShape(IRetangleShape* pRetangleShape)
{
    CHECH_INIT_STATE();
    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &RetangleShapeFactory::destroy, &m_RetangleShapeFactory,
                pRetangleShape);
}

void VideoRendererImpl::destroyAllRetangleShapes(void)
{
    CHECH_INIT_STATE();

    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << "destroy all RetangleShapes" );

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &RetangleShapeFactory::destroyAll,
                &m_RetangleShapeFactory);
}

IConvexShape* VideoRendererImpl::createConvexShape(uint32 pointCount)
{
    CHECH_INIT_STATE();

    return m_CommandQueue.blockingCall<IConvexShape*>(
                AsyncCommand::CT_DESTROY,
                &VideoRendererImpl::createConvexShapeCommand, this,
                pointCount);
}

void VideoRendererImpl::destroyConvexShape(IConvexShape* pConvexShape)
{
    CHECH_INIT_STATE();

    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << "destroy ConvexShape" );

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &ConvexShapeFactory::destroy, &m_ConvexShapeFactory,
                pConvexShape);
}

void VideoRendererImpl::destroyAllConvexShapes(void)
{
    CHECH_INIT_STATE();

    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << "destroy all ConvexShapes" );

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &ConvexShapeFactory::destroyAll, &m_ConvexShapeFactory);
}

ISprite* VideoRendererImpl::createSprite(const mf::FloatRect& rect)
{
    CHECH_INIT_STATE();
    return m_CommandQueue.blockingCall<ISprite*>(
                AsyncCommand::CT_CREATE,
                &VideoRendererImpl::createSpriteCommand, this,
                rect);
}

void VideoRendererImpl::destroySprite(ISprite* pSprite)
{
    CHECH_INIT_STATE();

    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << "destroy Sprite" );

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &SpriteFactory::destroy, &m_SpriteFactory,
                pSprite);
}

void VideoRendererImpl::destroyAllSprites(void)
{
    CHECH_INIT_STATE();

    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << "destroy all Sprites" );

    m_CommandQueue.blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &SpriteFactory::destroyAll,
                &m_SpriteFactory);
}

//========================================================================

void VideoRendererImpl::render(void)
{
    try
    {
        if(m_pRenderWindow && m_pRenderWindow->isOpen() )
        {
            GLVR_LOG_TRACE( "Video Render " << SMF_ADDR_PREFIX(this) << " render" );

            time_value presentationTime = m_pPresentationClock ?
                        m_pPresentationClock->getPresentationTime() : 0;

            for(VideoPaneImpl* pVideoPane : m_VideoPaneSet)
            {
                mf::IVideoBuffer* pVideoBuffer = pVideoPane->peekVideoData();
                if(pVideoBuffer)
                {                    
                    if(!m_pPresentationClock ||
                         pVideoBuffer->getTimestamp() - presentationTime <= 1)
                    {                        
                        pVideoPane->processQueuedDataCommand();

                        m_Updated = true;
                    }
                }
            }

            if(m_Updated)
            {
                m_pRenderWindow->clearFrameBuffer(Color::Black);

                GLVR_LOG_TRACE( "Video Render 0x" << this << " render" );

                m_pRenderWindow->render();

                m_pRenderWindow->swapBuffer();

                updateStatistics();

                m_Updated = false;
            }
         }
    }
    catch(glvr::GLVRException&)
    {
        m_Updated = false;
    }
    catch(std::exception& ex)
    {
        m_Updated = false;
        GLVR_LOG_ERROR("Catch std Exception: " << ex.what());
    }
    catch(...)
    {
        m_Updated = false;
        GLVR_LOG_ERROR("Catch Unknown Exception");
    }
}

bool VideoRendererImpl::isUpdated(void) const
{
    return m_Updated;
}

bool VideoRendererImpl::hasMoreCommand(void) const
{
    bool hasMoreData = false;
    for(VideoPaneImpl* pVideoPane : m_VideoPaneSet)
    {
        if(pVideoPane->hasMoreData())
        {
            hasMoreData = true;
        }
    }

    return m_CommandQueue.hasMoreCommand() || hasMoreData;
}


micro_seconds VideoRendererImpl::predictDelayTime(void)
{
    if(m_CommandQueue.hasMoreCommand())
        return 0;

    micro_seconds delay = 30000;

    micro_seconds presentationTime = m_pPresentationClock ? m_pPresentationClock->getPresentationTime() : 0;

    for(VideoPaneImpl* pVideoPane : m_VideoPaneSet)
    {
        if(mf::IVideoBuffer* pVideoBuffer = pVideoPane->peekVideoData())
        {
           delay = std::min(delay, pVideoBuffer->getTimestamp() - presentationTime - 1);
        }
    }

    return delay;
}

void VideoRendererImpl::executeCommands(void)
{
    if(m_Initialized && m_pRenderWindow && m_pRenderWindow->isOpen())
    {
        m_pRenderWindow->makeCurrent(true);
    }

    while(m_CommandQueue.hasMoreCommand())
    {
        mf::AsyncCommandPtr pCommand = m_CommandQueue.getNextCommand();

        if(pCommand->getType() != AsyncCommand::CT_GET)
            m_Updated = true;

        pCommand->invoke();
    }
}

//=========================================================================

void VideoRendererImpl::initializeCommand(mf::WindowHandle handle)
{    
    m_pRenderWindow = RenderEngine::getInstance().createRenderWindow(handle);

    resetStatistics();

    m_Updated = true;

    m_Initialized = true;

    GLVR_LOG_INFO("Video Render " <<  SMF_ADDR_PREFIX(this) << " is initialized");
}

void VideoRendererImpl::shutdownCommand(void)
{
    if(m_Initialized)
    {               
        GLVR_LOG_INFO("Shutdown Video Render " <<  SMF_ADDR_PREFIX(this));
        // All renderable must be destroy in RendererWindow is active
        m_pRenderWindow->makeCurrent(true);

        destroyAllRenderablesCommand();

        RenderEngine::getInstance().destroyRenderWindow(m_pRenderWindow);
        m_pRenderWindow = nullptr;

        m_Initialized = false;
    }
}

IVideoPane* VideoRendererImpl::createVideoPaneCommand(void)
{
    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " create VideoPane" );

    VideoPaneImpl* pVideoPaneI = new VideoPaneImpl(m_pRenderWindow, &m_CommandQueue);

    m_VideoPaneSet.insert(pVideoPaneI);

    return pVideoPaneI;
}

void VideoRendererImpl::destroyVideoPaneCommand(IVideoPane* pVideoPane)
{
    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " destroy VideoPane" );

    VideoPaneImpl* pVideoPaneImpl =  static_cast<VideoPaneImpl*>(pVideoPane);
    auto it = m_VideoPaneSet.find(pVideoPaneImpl);
    if(it != m_VideoPaneSet.end())
    {
        delete *it;
        m_VideoPaneSet.erase(it);
    }
    else
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Specified pointer does not exist!");
    }
}

void VideoRendererImpl::destroyAllVideoPanesCommand(void)
{
    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " destroy all VideoPanes" );

    mf::ContainerUtil::destroyAllElements(m_VideoPaneSet);
}

void VideoRendererImpl::flushCommand(void)
{
    for(VideoPaneImpl* pPane : m_VideoPaneSet)
    {
        pPane->flushCommnad();
    }
}

IRetangleShape* VideoRendererImpl::createRetangleShapeCommand(const mf::FloatRect& rect)
{
    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " create RetangleShape" );

    return m_RetangleShapeFactory.create<RetangleShapeImpl>(m_pRenderWindow, &m_CommandQueue, rect);
}

IConvexShape* VideoRendererImpl::createConvexShapeCommand(uint32 pointCount)
{
    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " create ConvexShape" );

    return m_ConvexShapeFactory.create<ConvexShapeImpl>(m_pRenderWindow, &m_CommandQueue, pointCount);
}

ISprite* VideoRendererImpl::createSpriteCommand(const mf::FloatRect& rect)
{
    GLVR_LOG_INFO( "Video Render " << SMF_ADDR_PREFIX(this) << " create Sprite" );

    return m_SpriteFactory.create<SpriteImpl>(m_pRenderWindow, &m_CommandQueue, rect);
}

void VideoRendererImpl::destroyAllRenderablesCommand(void)
{
    destroyAllVideoPanesCommand();

    m_RetangleShapeFactory.destroyAll();

    m_ConvexShapeFactory.destroyAll();

    m_SpriteFactory.destroyAll();
}



void VideoRendererImpl::emptyCommand(void)
{
    // do nothing because add a command to commad queue make repaint.
}

void VideoRendererImpl::setViewportCommand(const mf::FloatRect& rect)
{
    m_pRenderWindow->setViewport(rect);
}

const IVideoRenderer::Statistics& VideoRendererImpl::getStatisticsCommand(void) const
{
    return m_Statistics;
}

void VideoRendererImpl::setPresentationClockCommand(mf::IPresentationClock* pPresentationClock)
{
    m_pPresentationClock = pPresentationClock;
}

mf::IVideoBuffer* VideoRendererImpl::createPixmapVideoBufferCommand(uint16 width, uint16 height)
{
    return m_HardwareVideoBufferFactory.create<HardwareVideoBuffer>(
                m_pRenderWindow->createHardwarePixelBuffer(width, height));
}

void VideoRendererImpl::destroyPixmapVideoBufferCommand(mf::IVideoBuffer* pVideoBuffer)
{
    m_HardwareVideoBufferFactory.destroy(pVideoBuffer);
}

IDeviceManager* VideoRendererImpl::getDeviceManagerCommand(void)
{
    return m_pRenderWindow->getDeviceManager();
}

void VideoRendererImpl::resetStatistics(void)
{
    m_LastFramerateCounter.reset();
    m_AvgFramerateCounter.reset();

    m_Statistics.lastFPS = 0.0f;
    m_Statistics.avgFPS = 0.0f;
}

void VideoRendererImpl::updateStatistics(void)
{
    int64 currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    if(m_LastFramerateCounter.update(currentTime))
    {
        m_Statistics.lastFPS = m_LastFramerateCounter.getFPS();
        GLVR_LOG_TRACE("Video Render " << SMF_ADDR_PREFIX(this) <<" fps = " << m_Statistics.lastFPS);
    }

    if(m_AvgFramerateCounter.update(currentTime))
    {
        m_Statistics.avgFPS = m_LastFramerateCounter.getFPS();
        GLVR_LOG_TRACE("Video Render " << SMF_ADDR_PREFIX(this) << " avg fps = " << m_Statistics.avgFPS);
    }
}


} // namespace glvr
} // namespace surveon

