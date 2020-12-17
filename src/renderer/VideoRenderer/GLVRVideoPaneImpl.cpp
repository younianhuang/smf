#include "GLVRVideoPaneImpl.h"
#include "GLVRPixmapVideoBuffer.h"

namespace surveon
{

namespace glvr
{

        VideoPaneImpl::VideoPaneImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue) :
            m_pRenderTarget(pRenderTarget),
            m_pCommandQueue(pCommandQueue),
            m_pCurrentProcessedVideoBuffer(nullptr),
            m_pRetangleShape(new RetangleShape(pRenderTarget, pRenderTarget->getViewport().getRect())),
            m_pHostVideoPane(nullptr),
            m_VideoFormat(mf::SMF_SUB_TYPE_NONE),
            m_VideoWidth(0),
            m_VideoHeight(0)

        {            
            assert(pRenderTarget);
            assert(pCommandQueue);

            GLVR_LOG_INFO( "Video Pane "  << SMF_ADDR_PREFIX(this) << " is created");
        }

        VideoPaneImpl::~VideoPaneImpl(void)
        {
            m_pMaterial.reset();

            delete m_pRetangleShape;

            flushCommnad();

            GLVR_LOG_INFO( "Video Pane " << SMF_ADDR_PREFIX(this) << " is destroyed");
        }

        void VideoPaneImpl::processData(mf::IVideoBuffer* pVideoBuffer)
        {
            if(!pVideoBuffer)
            {
                GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Input pointer is null pointer!");
            }

            if(m_pHostVideoPane)
            {
                GLVR_THROW_EXCEPT(GLVRInvalidOperationException, "VideoPanel cannot process video data when it shares stream form other!");
            }

            pVideoBuffer->setReferenced(true);

            m_pCommandQueue->asyncCall<void>(
                               AsyncCommand::CT_GET,
                               &VideoBufferQueue::add, &m_VideoBufferQueue, pVideoBuffer);
        }

        void VideoPaneImpl::shareStreamFrom(IVideoPane* pVideoPane)
        {
            if(!pVideoPane)
            {
                GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Input pointer is null pointer!");
            }

            m_pCommandQueue->asyncCall<void>(
                       AsyncCommand::CT_SET,
                       &VideoPaneImpl::shareStreamFromCommand, this,
                        static_cast<VideoPaneImpl*>(pVideoPane));
        }

        void VideoPaneImpl::stopShareStream(void)
        {
            m_pCommandQueue->asyncCall<void>(
                       AsyncCommand::CT_SET,
                       &VideoPaneImpl::stopShareStreamCommand, this);
        }

        void VideoPaneImpl::setSourceRect(const mf::FloatRect& normalizedRect)
        {
            m_pCommandQueue->asyncCall<void>(
                       AsyncCommand::CT_SET,
                       &VideoPaneImpl::setSourceRectCommand, this,
                        normalizedRect);
        }

        mf::FloatRect VideoPaneImpl::getSourceRect(void) const
        {
            return m_pCommandQueue->blockingCall<mf::FloatRect>(
                        AsyncCommand::CT_GET,
                        &VideoPaneImpl::getSourceRectCommand, this);
        }

        void VideoPaneImpl::setDestinationRect(const mf::FloatRect& rect)
        {
            m_pCommandQueue->asyncCall<void>(
                       AsyncCommand::CT_SET,
                       &VideoPaneImpl::setDestinationRectCommand, this,
                        rect);
        }

        mf::FloatRect VideoPaneImpl::getDestinationRect(void) const
        {
            return m_pCommandQueue->blockingCall<mf::FloatRect>(
                        AsyncCommand::CT_GET,
                        &VideoPaneImpl::getDestinationRectCommand, this);
        }

        void VideoPaneImpl::flush(void)
        {
            m_pCommandQueue->blockingCall<void>(
                                    AsyncCommand::CT_SET,
                                    &VideoPaneImpl::flushCommnad, this);
        }

        void VideoPaneImpl::flushCommnad(void)
        {
            while(m_VideoBufferQueue.hasMore())
            {
                IVideoBuffer* pVideoBuffer = m_VideoBufferQueue.getNext();

                pVideoBuffer->setReferenced(false);
            }
        }

        void VideoPaneImpl::setColor(const Color& color)
        {
            m_pRetangleShape->setFillColor(color);
        }

        uint8 VideoPaneImpl::getZOrder(void) const
        {
            return m_pCommandQueue->blockingCall<uint8>(
                        AsyncCommand::CT_GET,
                        &RetangleShape::getZOrder, m_pRetangleShape);
        }

        void VideoPaneImpl::setZOrder(uint8 zorder)
        {
            m_pCommandQueue->asyncCall<void>(
                       AsyncCommand::CT_SET,
                       &VideoPaneImpl::setZOrderCommand, this,
                        zorder);
        }

        void VideoPaneImpl::show(void)
        {
            m_pCommandQueue->asyncCall<void>(
                       AsyncCommand::CT_SET,
                       &RetangleShape::show, m_pRetangleShape);
        }

        void VideoPaneImpl::hide(void)
        {
            m_pCommandQueue->addCommand<void>(
                        AsyncCommand::CT_SET,
                        &RetangleShape::hide, m_pRetangleShape);
        }

        bool VideoPaneImpl::isShow(void) const
        {
            return m_pCommandQueue->blockingCall<bool>(
                                    AsyncCommand::CT_GET,
                                    &RetangleShape::isShow, m_pRetangleShape);
        }

        void VideoPaneImpl::setBorderColor(const Color& color)
        {
            m_pCommandQueue->asyncCall<void>(
                        AsyncCommand::CT_SET,
                        &RetangleShape::setOutlineColor, m_pRetangleShape,
                        color);
        }

        void VideoPaneImpl::setBorderThickness(int thickness)
        {
            m_pCommandQueue->asyncCall<void>(
                        AsyncCommand::CT_SET,
                        &RetangleShape::setOutlineThickness, m_pRetangleShape,
                        thickness);
        }

        void VideoPaneImpl::processQueuedDataCommand(void)
        {            
            m_pCurrentProcessedVideoBuffer = m_VideoBufferQueue.getNext();

            try
            {
                if(!m_pMaterial ||
                     m_VideoFormat != m_pCurrentProcessedVideoBuffer->getVideoFormat() ||
                     m_VideoWidth != m_pCurrentProcessedVideoBuffer->getWidth() ||
                     m_VideoHeight != m_pCurrentProcessedVideoBuffer->getHeight()  )
                {
                    MaterialSharedPtr pMaterial = createMaterial(m_pCurrentProcessedVideoBuffer);

                    setMaterial(pMaterial);

                    m_VideoWidth = m_pCurrentProcessedVideoBuffer->getWidth();

                    m_VideoHeight = m_pCurrentProcessedVideoBuffer->getHeight();

                    m_VideoFormat = m_pCurrentProcessedVideoBuffer->getVideoFormat();
                }

                uploadVideoData(m_pCurrentProcessedVideoBuffer);
            }
            catch(...)
            {
                // prevent video buffer is blocked
                if(m_pCurrentProcessedVideoBuffer->isLocked())
                {
                    m_pCurrentProcessedVideoBuffer->unlockPlane();
                }

                m_pCurrentProcessedVideoBuffer->setReferenced(false);

                throw;
            }
        }

        mf::FloatRect VideoPaneImpl::getBoundaryRect(void) const
        {
            return m_pCommandQueue->blockingCall<mf::FloatRect>(
                        AsyncCommand::CT_GET,
                        &RetangleShape::getBoundaryRect, m_pRetangleShape);
        }

        //==========================================================================================        

        void VideoPaneImpl::setSourceRectCommand(const mf::FloatRect& rect)
        {
            GLVR_LOG_DEBUG("Source Rect left = " << rect.left <<
                                              " top = "<< rect.top <<
                                              " width = " << rect.right - rect.left <<
                                               " height = " << rect.bottom -rect.top);

            m_pRetangleShape->setTextureCoordRect(rect);
        }

        mf::FloatRect VideoPaneImpl::getSourceRectCommand(void) const
        {
            return m_pRetangleShape->getTextureCoordRect();
        }

        void VideoPaneImpl::setDestinationRectCommand(const mf::FloatRect& rect)
        {            
            GLVR_LOG_DEBUG("Video Pane 0x" << this <<
                                              " Dest Rect left = " << rect.left <<
                                              " top = "<< rect.top <<
                                              " width = " << rect.right - rect.left <<
                                               " height = " << rect.bottom -rect.top);

            m_pRetangleShape->setRect(rect);
        }

        mf::FloatRect VideoPaneImpl::getDestinationRectCommand(void) const
        {
            return m_pRetangleShape->getRect();
        }

        void VideoPaneImpl::setZOrderCommand(uint8 zorder)
        {
            GLVR_LOG_DEBUG("Video Pane 0x" << this <<
                                              " ZOrder = " << zorder);

            m_pRetangleShape->setZOrder(zorder);
        }

        uint8 VideoPaneImpl::getZOrderCommand(void) const
        {
            return m_pRetangleShape->getZOrder();
        }

        void VideoPaneImpl::shareStreamFromCommand(VideoPaneImpl* pVideoPane)
        {
            if(m_GuestVideoPanes.size())
            {
                GLVR_THROW_EXCEPT(GLVRInvalidStateException, "This VideoPaneI already shares stream to others!");
            }

            m_pHostVideoPane = pVideoPane;

            pVideoPane->addGuest(this);

            setMaterial(m_pHostVideoPane->m_pMaterial);
        }

        void VideoPaneImpl::stopShareStreamCommand(void)
        {
            m_pHostVideoPane->removeGuest(this);

            m_pHostVideoPane = nullptr;

            setMaterial(MaterialSharedPtr());
        }

        MaterialSharedPtr VideoPaneImpl::createMaterial(mf::IVideoBuffer* pVideoBuffer)
        {
            MaterialSharedPtr pMaterial(new Material);

            switch(pVideoBuffer->getVideoFormat())
            {
            case mf::SMF_VIDEO_FORMAT_RGBA:
                {
                    TextureSharedPtr pTexture = TextureManager::getInstance().createTexture(
                                Texture::PF_RGBA, pVideoBuffer->getWidth(), pVideoBuffer->getHeight());

                    pMaterial->addTextureUnitState(TextureUnitState{pTexture});

                    GPUProgramSharedPtr pProgram = GPUProgramManager::getInstancePtr()->getRGBTextureGPUProgram();
                    pProgram->setParameter("sampler0", 0);
                    pMaterial->setProgram(pProgram);
                }
                break;


            case mf::SMF_VIDEO_FORMAT_YUV420:
            case mf::SMF_VIDEO_FORMAT_YV12:
                {
                    TextureSharedPtr pYTexture = TextureManager::getInstance().createTexture(
                                Texture::PF_RED, pVideoBuffer->getWidth(), pVideoBuffer->getHeight());
                    TextureSharedPtr pUTexture = TextureManager::getInstance().createTexture(
                                Texture::PF_RED, pVideoBuffer->getWidth() >> 1, pVideoBuffer->getHeight() >> 1);
                    TextureSharedPtr pVTexture = TextureManager::getInstance().createTexture(
                                Texture::PF_RED, pVideoBuffer->getWidth() >> 1, pVideoBuffer->getHeight() >> 1);


                    pMaterial->addTextureUnitState(TextureUnitState{pYTexture});
                    pMaterial->addTextureUnitState(TextureUnitState{pUTexture});
                    pMaterial->addTextureUnitState(TextureUnitState{pVTexture});

                    GPUProgramSharedPtr pProgram = GPUProgramManager::getInstancePtr()->getYUV420TextureGPUProgram();

                    pProgram->setParameter("sampler0", 0);
                    pProgram->setParameter("sampler1", 1);
                    pProgram->setParameter("sampler2", 2);

                    pMaterial->setProgram(pProgram);
                }
                break;

            case mf::SMF_VIDEO_FORMAT_XPIXMAP:
            case mf::SMF_VIDEO_FORMAT_D3D9_SURFACE:
                {
                    TextureSharedPtr pTexture = TextureManager::getInstance().createTexture();

                    HardwarePixelBufferSharedPtr pPixelBuffer =
                            m_pRenderTarget->createHardwarePixelBuffer(pVideoBuffer->getWidth(), pVideoBuffer->getHeight());

                    pTexture->create(pPixelBuffer);

                    pMaterial->addTextureUnitState(TextureUnitState{pTexture});

                    GPUProgramSharedPtr pProgram = GPUProgramManager::getInstancePtr()->getRGBTextureGPUProgram();
                    pProgram->setParameter("sampler0", 0);
                    pMaterial->setProgram(pProgram);
                }
                break;
            }

            return pMaterial;
        }

        void VideoPaneImpl::setMaterial(MaterialSharedPtr pMaterial)
        {
            m_pMaterial = pMaterial;

            m_pRetangleShape->setMaterial(pMaterial);

            // notify slave video pane
            for(VideoPaneImpl* pGuest : m_GuestVideoPanes)
            {
                pGuest->setMaterial(m_pMaterial);
            }
        }

        void VideoPaneImpl::uploadVideoData(mf::IVideoBuffer* pVideoBuffer)
        {
            const PlaneInfo&  planeInfo = pVideoBuffer->lockPlane();

            switch(pVideoBuffer->getVideoFormat())
            {
            case mf::SMF_VIDEO_FORMAT_RGBA:
                {
                    const PlaneInfo::PlaneData& planData = planeInfo.getPlaneData(0);

                    TextureSharedPtr pTexture = m_pMaterial->getTexture(0);
                    pTexture->upload(planData.pixels, Texture::PF_RGBA);
                }
                break;
            case mf::SMF_VIDEO_FORMAT_YUV420:
                {
                    TextureSharedPtr pYTexture = m_pMaterial->getTexture(0);
                    pYTexture->upload(planeInfo.getPlaneData(0).pixels, Texture::PF_RED);

                    TextureSharedPtr pUTexture = m_pMaterial->getTexture(1);
                    pUTexture->upload(planeInfo.getPlaneData(1).pixels, Texture::PF_RED);

                    TextureSharedPtr pVTexture = m_pMaterial->getTexture(2);
                    pVTexture->upload(planeInfo.getPlaneData(2).pixels, Texture::PF_RED);
                }
                break;
            case mf::SMF_VIDEO_FORMAT_YV12:
                {
                    TextureSharedPtr pYTexture = m_pMaterial->getTexture(0);
                    pYTexture->upload(planeInfo.getPlaneData(0).pixels, Texture::PF_RED);

                    TextureSharedPtr pUTexture = m_pMaterial->getTexture(1);
                    pUTexture->upload(planeInfo.getPlaneData(2).pixels, Texture::PF_RED);

                    TextureSharedPtr pVTexture = m_pMaterial->getTexture(2);
                    pVTexture->upload(planeInfo.getPlaneData(1).pixels, Texture::PF_RED);
                }
                break;

            case mf::SMF_VIDEO_FORMAT_XPIXMAP:
            case mf::SMF_VIDEO_FORMAT_D3D9_SURFACE:
                {
                    TextureSharedPtr pTexture = m_pMaterial->getTexture(0);

                    HardwarePixelBufferSharedPtr pSrc = static_cast<HardwareVideoBuffer*>(pVideoBuffer)->getPixelBuffer();

                    pTexture->blit(pSrc);
                }

                break;
            }

            pVideoBuffer->unlockPlane();

            pVideoBuffer->setReferenced(false);
        }

} // namespace glvr
} // namespace surveon
