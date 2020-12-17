#include "ViewTile.h"
#include <QResizeEvent>


#define DECODER_VO 0

using namespace surveon;
using surveon::mf::String;

ViewTile::ViewTile(QWidget *parent) :
    QWidget(parent),
    m_Run(false),
    m_FirstPacket(true),
    m_RenderingVideo(true),
    m_DecodeVideo(true),
    m_DownScaling(true),
    m_WindowWidth(0),
    m_WindowHeight(0),
    m_pSource(nullptr),
    m_pDecoder(nullptr),
    m_pVideoRenderer(nullptr),
    m_pVideoPane(nullptr),
    m_pClock(nullptr),
    m_pPresentationClock(nullptr),
    m_TimingControl(true),
    m_VideoBuffers(1),
    m_VideoBufferIndex(0),
    m_pCurrentCompressedVideoBuffer(nullptr),
    m_pCurrentDecodedVideoBuffer(nullptr),
    m_Rate(1.3f),
    m_FileSource(QString()),
    m_HardwareDecode(false)
{           

    // Setup some states to allow direct rendering into the widget

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);
}

ViewTile::~ViewTile()
{

}

void ViewTile::setFileSource(const QString& filename)
{
    m_FileSource = filename;
}

void ViewTile::closeFile()
{
    if (!m_Run)
        return;

    m_Run = false;
    m_Thread.join();

    mf::MediaManager* pMediaManager = mf::MediaManager::getInstancePtr();

    pMediaManager->destroyMediaSource(m_pSource);
    pMediaManager->destroyMediaTransform(m_pDecoder);

    m_pVideoRenderer->flush();

    destroyVideoBuffers();

    glvr::VideoRendererManager::getInstancePtr()->destroyVideoRenderer(m_pVideoRenderer);

    if(m_pPresentationClock)
    {
        pMediaManager->destroyPresentationClock(m_pPresentationClock);
        pMediaManager->destroyClock(m_pClock);
    }

    m_FirstPacket = true;
}

QString ViewTile::getCodec(void)
{
    QString codec("Invalid");

    for(size_t idx = 0; idx<m_pSource->getNumOfStream(); idx++ )
    {
        mf::IStreamSource* pStreamSource =  m_pSource->getStreamSource(idx);
        if(pStreamSource && pStreamSource->getMediaType().majoryType == mf::SMF_MT_VIDEO)
        {
            switch ( pStreamSource->getMediaType().subType )
            {
                case mf::SMF_VIDEO_FORMAT_MJPEG:    codec = "MJPEG";    break;
                case mf::SMF_VIDEO_FORMAT_MPEG4:    codec = "MPEG4";    break;
                case mf::SMF_VIDEO_FORMAT_H264:     codec = "H264";     break;
                case mf::SMF_VIDEO_FORMAT_RGBA:     codec = "RGBA";     break;
                case mf::SMF_VIDEO_FORMAT_YUV420:   codec = "YUV420";   break;
                case mf::SMF_VIDEO_FORMAT_YV12:     codec = "YV12";     break;
                case mf::SMF_VIDEO_FORMAT_VDPAU:    codec = "VDPAU";    break;
                case mf::SMF_VIDEO_FORMAT_VAAPI:    codec = "VAAPI";    break;
                case mf::SMF_VIDEO_FORMAT_XPIXMAP: codec = "GLPIXMAP"; break;
            }
        }
    }

    return codec;
}

void ViewTile::openFile()
{
    mf::MediaManager* pMediaManager = mf::MediaManager::getInstancePtr();

    m_pVideoRenderer = glvr::VideoRendererManager::getInstancePtr()->createVideoRenderer();

    m_pVideoRenderer->initialize(surveon::mf::WindowHandle(winId()));

    m_pVideoPane = m_pVideoRenderer->createVideoPane();

    // Timing Control
    m_pClock =  pMediaManager->createClock(mf::CT_HIGH_RESOLUTION_CLOCK);
    m_pPresentationClock = pMediaManager->createPresentationClock();
    m_pPresentationClock->setTimeSource(m_pClock);
    m_pPresentationClock->setRate(m_TimingControl ? m_Rate : FLOAT_MAX);
    m_pVideoRenderer->setPresentationClock(m_pPresentationClock);

    if(strncmp(m_FileSource.toStdString().c_str(), "rtsp", 4) == 0)
    {
        // Network stream
        m_pSource = pMediaManager->createSourceFromURL( m_FileSource.toStdString() );
    }
    else
    {
        // Local file
        m_pSource = pMediaManager->createSourceFromFile( m_FileSource.toStdString() );
    }

    for(size_t idx = 0; idx<m_pSource->getNumOfStream(); idx++ )
    {
        mf::IStreamSource* pStreamSource =  m_pSource->getStreamSource(idx);
        if(pStreamSource && pStreamSource->getMediaType().majoryType == mf::SMF_MT_VIDEO)
        {
            m_pDecoder = pMediaManager->createMediaTransform("ffmpegVideoDecoder");

            setDecoderHardwareDecode();

#if DECODER_VO

            m_pDecoder->getAttributes()->setAttribuate(mf::ATTRI_VO_TARGET, mf::WindowHandle(winId()));
#endif
            m_pDecoder->initialize();
            m_pDecoder->setInputType(pStreamSource->getMediaType());
        }
    }

    m_Run = true;
    m_Thread = mf::Thread(&ViewTile::processVideo, this);

    setupDisplayRect(this->size());
}

void ViewTile::setHardwareDecode(bool hardwareDecode)
{
    m_HardwareDecode = hardwareDecode;
}

void ViewTile::setDecoderHardwareDecode(void)
{
    mf::Attributes* pAttributes = m_pDecoder->getAttributes();
    pAttributes->setAttribuate(mf::ATTRI_HARDWARD_ACCEL, m_HardwareDecode);

    if(m_HardwareDecode)
    {
        // for tht interoperation of hardware deocde and GLVR
        mf::IDeviceManager* pDeviceManager = m_pVideoRenderer->getDeviceManager();

        m_pDecoder->getAttributes()->setAttribuate(mf::ATTRI_DEVICE_MANAGER, pDeviceManager);
    }
}

void ViewTile::setupDisplayRect(const QSize& size)
{
    if(m_pVideoRenderer)
    {
        surveon::mf::FloatRect viewport(0, 0, size.width(), size.height());

        m_pVideoRenderer->setViewport(viewport);

        if(m_pVideoPane)
        {
            surveon::mf::FloatRect destRect(0, 0, size.width(), size.height());
            m_pVideoPane->setDestinationRect(destRect);

            m_pVideoPane->setSourceRect(surveon::mf::FloatRect(0.0, 0.0, 1.0, 1.0));
        }
    }
}

void ViewTile::startPlayVideo()
{
    m_pSource->start();

    // wait until all video buffer are rendered
    for(size_t i=0; i<m_VideoBuffers.size(); i++)
    {
        mf::IVideoBuffer* pVideoBuffer = m_VideoBuffers[i];

        if(pVideoBuffer)
            pVideoBuffer->waitUntilFree();
    }

    if(m_pPresentationClock)
        m_pPresentationClock->start();
}

void ViewTile::processVideo(void)
{    
    startPlayVideo();

    while(m_Run)
    {
        mf::IMediaSource::StreamStatus status = m_pSource->getPacket(&m_pCurrentCompressedVideoBuffer);
        if(m_DecodeVideo && status == mf::IMediaSource::SS_OK)
        {
            if(m_pCurrentCompressedVideoBuffer)
            {
                decodeVideo();

                if(m_pDecoder->getOutputStatus() == mf::IMediaTransform::ST_OUTPUT_DATA_READY )
                {
                    if(m_RenderingVideo)
                    {
                        // get stream info after decode first packet
                        if(m_FirstPacket)
                        {
                            createVideoBuffers();
                            m_FirstPacket = false;
                        }

                        m_pDecoder->processOutput(m_pCurrentDecodedVideoBuffer);

#if DECODER_VO


#else

                        if(m_pCurrentDecodedVideoBuffer)
                        {
                            renderVideo();
                        }
#endif
                    }

                    // block until there is buffer to render
                    m_pCurrentDecodedVideoBuffer = getNextVideoBuffer();

                    if(m_pCurrentDecodedVideoBuffer)
                        m_pCurrentDecodedVideoBuffer->waitUntilFree();

                }
            }
            else
            {
                m_SyncControl.waitCommandForMilliSeconds(10);
            }
        }
        else if(status == mf::IMediaSource::SS_END_OF_STREAM)
        {            
            // restart reading avi from the begin
            startPlayVideo();
        }        
    }
}

surveon::mf::IVideoBuffer* ViewTile::getNextVideoBuffer(void)
{
    mf::IVideoBuffer* pVideoBuffer =  m_VideoBuffers[m_VideoBufferIndex];
    if(++m_VideoBufferIndex >= m_VideoBuffers.size())
    {
        m_VideoBufferIndex = 0;
    }

    return pVideoBuffer;
}

void  ViewTile::decodeVideo(void)
{
    if(m_pDecoder->getInputStatus() == mf::IMediaTransform::ST_ACCEPT_DATA)
    {
        m_pDecoder->processInput(m_pCurrentCompressedVideoBuffer);        
    }
}

void ViewTile::renderVideo(void)
{        
    m_pVideoPane->processData(m_pCurrentDecodedVideoBuffer);
}

void ViewTile::createVideoBuffers(void)
{    
    mf::IMediaTransform::StreamInfo streamInfo;

    m_pDecoder->getOutputStreamInfo(streamInfo);

    mf::uint16 width = 0;
    mf:: uint16 height = 0;

    if(m_DownScaling)
    {
        width = std::max<mf::uint16>(320, m_WindowWidth + 7 & 0xfff8);
        height = std::max<mf::uint16>(240, m_WindowHeight + 7 & 0xfff8);
    }
    else
    {
        width = streamInfo.width;
        height = streamInfo.height;
    }

    for(size_t i=0; i<m_VideoBuffers.size(); i++)
    {
        if(streamInfo.mediaType.subType ==  mf::SMF_VIDEO_FORMAT_VAAPI ||
            streamInfo.mediaType.subType ==  mf::SMF_VIDEO_FORMAT_DXVA2 )
        {            
            m_VideoBuffers[i] = m_pVideoRenderer->createGPUHardwareVideoBuffer(width, height);
        }        
        else
        {            
            m_VideoBuffers[i] = mf::getSystemMemoryMediaBufferFactory()->createVideoBuffer(
                    width, height, streamInfo.mediaType.subType);            
        }

    }

    m_pCurrentDecodedVideoBuffer = getNextVideoBuffer();
}

void ViewTile::destroyVideoBuffers(void)
{
    for(size_t i=0; i<m_VideoBuffers.size(); i++)
    {
        mf::IVideoBuffer*pVideoBuffer = m_VideoBuffers[i];

        if(pVideoBuffer)
        {
            if(pVideoBuffer->getVideoFormat() == mf::SMF_VIDEO_FORMAT_XPIXMAP ||
                pVideoBuffer->getVideoFormat() == mf::SMF_VIDEO_FORMAT_D3D9_SURFACE )
            {
                m_pVideoRenderer->destroyGPUHardwareVideoBuffer(pVideoBuffer);

                m_VideoBuffers[i] = nullptr;
            }
            else
            {
                mf::getSystemMemoryMediaBufferFactory()->destroyVideoBuffer(pVideoBuffer);

                m_VideoBuffers[i] = nullptr;
            }
        }
    }
}

void ViewTile::closeEvent(QCloseEvent *)
{
    closeFile();
}

void ViewTile::resizeEvent (QResizeEvent * event)
{
    setupDisplayRect(event->size());

     QWidget::resizeEvent(event);

     m_WindowWidth = event->size().width();
     m_WindowHeight = event->size().height();
}

void ViewTile::showEvent(QShowEvent*)
{
    setupDisplayRect(this->size());
}

float ViewTile::getRendererFPS() const
{
    return m_pVideoRenderer->getStatistics().lastFPS;
}

void ViewTile::setRenderingVideo(bool renderingVideo)
{
    m_RenderingVideo = renderingVideo;
    if(!m_RenderingVideo && m_pVideoRenderer)
    {
        m_pVideoRenderer->flush();
    }
}

void ViewTile::setDecodeVideo(bool decodeVideo)
{
    m_DecodeVideo = decodeVideo;
}

void ViewTile::setDownScaling(bool downScaling)
{
    m_DownScaling = downScaling;
}

float ViewTile::getDecoderFPS() const
{
    surveon::mf::IMediaTransform::Statistics stat;
    m_pDecoder->getStatistics(stat);

    return stat.lastFPS;
}

size_t ViewTile::getDecoderBitrate() const
{
    surveon::mf::IMediaTransform::Statistics stat;
    m_pDecoder->getStatistics(stat);

    return stat.bitrate;
}

float ViewTile::getSourceFPS() const
{
    for(size_t idx = 0; idx<m_pSource->getNumOfStream(); idx++ )
    {
        mf::IStreamSource* pStreamSource =  m_pSource->getStreamSource(idx);
        if(pStreamSource && pStreamSource->getMediaType().majoryType == mf::SMF_MT_VIDEO)
        {
            mf::IStreamSource::Statistics stat;
            pStreamSource->getStatistics(stat);

            return stat.lastFPS;
        }
    }

    return 0.0f;
}

size_t ViewTile::getSourceBitrate() const
{
    mf::IStreamSource::Statistics stat;
    for(size_t idx = 0; idx<m_pSource->getNumOfStream(); idx++ )
    {
        mf::IStreamSource* pStreamSource =  m_pSource->getStreamSource(idx);
        if(pStreamSource && pStreamSource->getMediaType().majoryType == mf::SMF_MT_VIDEO)
        {            
            pStreamSource->getStatistics(stat);
            break;
        }
    }

    return stat.bitrate;
}

void ViewTile::setTimingControl(bool timingControl)
{
    m_TimingControl = timingControl;

    m_pPresentationClock->setRate(m_TimingControl ? m_Rate : FLOAT_MAX);
}

void ViewTile::setRate(float rate)
{
    m_Rate = rate;

    if (m_TimingControl)
        m_pPresentationClock->setRate(m_Rate);
}

void ViewTile::stopSource(void)
{
    m_pSource->stop();
}

void ViewTile::startSource(void)
{
    m_pSource->start();
}

void ViewTile::pauseSource(void)
{
    m_pSource->pause();
}

void ViewTile::resumeSource(void)
{
    m_pSource->resume();
}

QString ViewTile::getVideoResolution(void)
{
    mf::IMediaTransform::StreamInfo streamInfo;

    m_pDecoder->getOutputStreamInfo(streamInfo);

    return QString("%1x%2").arg(streamInfo.width).arg(streamInfo.height);
}

