#include "SMFffmpegDecoder.h"
#include "SMFffmpegMappings.h"
#include "SMFffmpegManager.h"


namespace surveon
{
namespace mf
{

String ffmpegVideoDecoder::ms_TypeId = "ffmpegVideoDecoder";

//======================================================================

ffmpegVideoDecoder::ffmpegVideoDecoder(AsyncCommandQueue* pCommandQueue) :
    m_pAVCodecContext(nullptr),
    m_pAVCodec(nullptr),
    m_AVOpts(nullptr),
    m_pAVFrame(nullptr),
    m_pAVFrameScaleOut(nullptr),
    m_pSwsContext(nullptr),
    m_InputStatus(ST_OK),
    m_OutputStatus(ST_OK),
    m_AvgFramerateCounter(5000),
    m_FirstPacket(true),
    m_pCommandQueue(pCommandQueue),
    m_Timestamp(0),
    m_pHWAccel(nullptr),
    m_pDeviceManager(nullptr)
{
    SMF_FFMPEG_LOG_INFO( "Create ffmpegVideoDecoder 0x" << this );

    m_Attributes.setAttribuate(ATTRI_HARDWARD_ACCEL, false);
    m_Attributes.setAttribuate(ATTRI_VO_TARGET, mf::WindowHandle());
    m_Attributes.setAttribuate(ATTRI_DEVICE_MANAGER, m_pDeviceManager);
}

ffmpegVideoDecoder::~ffmpegVideoDecoder()
{
    shutdown();

    SMF_FFMPEG_LOG_INFO( "Destroy ffmpegVideoDecoder 0x" << this );
}

const String& ffmpegVideoDecoder::getTypeId(void) const
{
    return ffmpegVideoDecoder::typeId();
}

void ffmpegVideoDecoder::initialize(void)
{
    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_CREATE,
                &ffmpegVideoDecoder::initializeCommand, this);
}

void ffmpegVideoDecoder::shutdown(void)
{
    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_DESTROY,
                &ffmpegVideoDecoder::shutdownCommand, this);
}

void ffmpegVideoDecoder::setInputType(const MediaType& mediaType)
{
    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_SET,
                &ffmpegVideoDecoder::setInputTypeCommand, this, mediaType);
}

ffmpegVideoDecoder::Status ffmpegVideoDecoder::getInputStatus(void) const
{
    return m_pCommandQueue->blockingCall<Status>(
                AsyncCommand::CT_GET,
                &ffmpegVideoDecoder::getInputStatusCommand, this);
}

void ffmpegVideoDecoder::processInput(IMediaBuffer* pBuffer)
{              
    if(!pBuffer)
    {
        SMF_FFMPEG_THROW(InvalidParametersException, "Invalid null potiner");
    }

    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_SET,
                &ffmpegVideoDecoder::processInputCommand, this, pBuffer);
}

void ffmpegVideoDecoder::setOutputType(const MediaType& /*mediaType*/ )
{

}

void ffmpegVideoDecoder::getOutputStreamInfo(StreamInfo& streamInfo) const
{
    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_GET,
                &ffmpegVideoDecoder::getOutputStreamInfoCommand, this, &streamInfo);
}

ffmpegVideoDecoder::Status ffmpegVideoDecoder::getOutputStatus(void) const
{
    return m_pCommandQueue->blockingCall<Status>(
                AsyncCommand::CT_GET,
                &ffmpegVideoDecoder::getOutputStatusCommand, this);
}

void ffmpegVideoDecoder::processOutput(IMediaBuffer* pBuffer)
{
    if(!pBuffer)
    {
        SMF_FFMPEG_THROW(InvalidParametersException, "Invalid null potiner");
    }

    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_SET,
                &ffmpegVideoDecoder::processOutputCommand, this, pBuffer);
}

void ffmpegVideoDecoder::getStatistics(Statistics& stat) const
{
    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_GET,
                &ffmpegVideoDecoder::getStatisticsCommand, this, &stat);
}

Attributes* ffmpegVideoDecoder::getAttributes(void)
{
    return &m_Attributes;
}

//====================================================================
void ffmpegVideoDecoder::initializeCommand()
{
    m_InputStatus = ST_NOT_ACCEPT_DATA;

    m_Timestamp = 0;
}

void ffmpegVideoDecoder::shutdownCommand(void)
{
    destroyCodec();
}

void ffmpegVideoDecoder::setInputTypeCommand(const MediaType& mediaType)
{
    if(m_InputType.subType != mediaType.subType)
    {
        m_InputType = mediaType;

        bool enableHWAccel = m_Attributes.getAttribuate<bool>(ATTRI_HARDWARD_ACCEL);

        m_pDeviceManager = m_Attributes.getAttribuate<IDeviceManager*>(ATTRI_DEVICE_MANAGER);

        destroyCodec();
        createCodec(enableHWAccel);
    }
}

ffmpegVideoDecoder::Status ffmpegVideoDecoder::getInputStatusCommand(void) const
{
    return m_InputStatus;
}

void ffmpegVideoDecoder::processInputCommand(IMediaBuffer* pBuffer)
{
    int gotPicture;

    m_Packet.data = pBuffer->lock();
    m_Packet.size = static_cast<int>(pBuffer->getCurrentLength());

    int ret = avcodec_decode_video2(m_pAVCodecContext, m_pAVFrame, &gotPicture, &m_Packet);
    if( ret < 0)
    {
        SMF_FFMPEG_LOG_ERROR("fail to decode video packet : [" <<
                             ffmpegHelper::getErrorString(ret) << "] ");

        if(m_pHWAccel && m_pAVCodecContext->pix_fmt == AV_PIX_FMT_NONE)
        {
            SMF_FFMPEG_LOG_ERROR("fail to create hardware accelerator. Roll back to software codec");

            destroyCodec();
            createCodec(false);

            pBuffer->unlock();

            processInputCommand(pBuffer);

            pBuffer->lock();
        }
    }
    else
    {
        m_BitrateCounter.accumulate(ret << 3);

        m_Packet.size -= ret;
        assert(m_Packet.size == 0);

        if(m_FirstPacket)
        {
            updateStreamInfo();
            m_FirstPacket = false;
        }

        if(gotPicture)
        {
            updateStatistics();
            m_Timestamp = pBuffer->getTimestamp();
            m_OutputStatus =  ST_OUTPUT_DATA_READY;
        }
        else
        {
            m_OutputStatus =  ST_NEED_MORE_INPUT;
        }
    }

    pBuffer->unlock();
}

void ffmpegVideoDecoder::getOutputStreamInfoCommand(StreamInfo* pStreamInfo) const
{
    *pStreamInfo = m_StreamInfo;
}

ffmpegVideoDecoder::Status ffmpegVideoDecoder::getOutputStatusCommand(void) const
{
    return m_OutputStatus;
}

void ffmpegVideoDecoder::processOutputCommand(IMediaBuffer* pBuffer)
{
    if(m_OutputStatus == ST_OUTPUT_DATA_READY)
    {        
        if(m_pHWAccel)
        {
            m_pHWAccel->retriveVideoData(m_pAVFrame,
                                         static_cast<IVideoBuffer*>(pBuffer));

            pBuffer->setTimestamp(m_Timestamp);
        }
        else
        {
            IVideoBuffer* pVideoBuffer = static_cast<IVideoBuffer*>(pBuffer);
            const PlaneInfo&  planInfo = pVideoBuffer->lockPlane();

            PlaneInfo::PlaneData planData =  planInfo.getPlaneData(0);

            if(m_OutputFrameSize.width != pVideoBuffer->getWidth() ||
                m_OutputFrameSize.height != pVideoBuffer->getHeight())
            {
                sws_freeContext(m_pSwsContext);

                m_pSwsContext = sws_getContext(
                            m_pAVCodecContext->width,
                            m_pAVCodecContext->height,
                            m_pAVCodecContext->pix_fmt,
                            pVideoBuffer->getWidth(),
                            pVideoBuffer->getHeight(),
                            m_pAVCodecContext->pix_fmt,
                            SWS_FAST_BILINEAR,
                            NULL,
                            NULL,
                            NULL );

                m_OutputFrameSize.width = pVideoBuffer->getWidth();
                m_OutputFrameSize.height = pVideoBuffer->getHeight();
            }

            avpicture_fill(
                        (AVPicture *)m_pAVFrameScaleOut,
                        planData.pixels,
                        m_pAVCodecContext->pix_fmt,
                        planData.width,
                        planData.height);

            sws_scale(m_pSwsContext, (uint8_t const * const *)m_pAVFrame->data,
                      m_pAVFrame->linesize, 0, m_pAVCodecContext->height,
                      m_pAVFrameScaleOut->data, m_pAVFrameScaleOut->linesize);

            pVideoBuffer->unlock();

            pVideoBuffer->setTimestamp(m_Timestamp);
        }

        m_OutputStatus = ST_NEED_MORE_INPUT;
    }
}

void ffmpegVideoDecoder::getStatisticsCommand(Statistics* pStat) const
{
    *pStat = m_Statistics;
}

void ffmpegVideoDecoder::createCodec(bool enableHWAccel)
{        
    switch(m_InputType.subType)
    {
    case SMF_VIDEO_FORMAT_MJPEG:
        SMF_FFMPEG_LOG_INFO( "ffmpegVideoDecoder "<< SMF_ADDR_PREFIX(this) << " open MJPEG codec" );
        m_pAVCodec = avcodec_find_decoder(AV_CODEC_ID_MJPEG);
        break;
    case SMF_VIDEO_FORMAT_MPEG4:
        SMF_FFMPEG_LOG_INFO( "ffmpegVideoDecoder " << SMF_ADDR_PREFIX(this) << " open MPEG4 codec" );
        m_pAVCodec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
        break;
    case SMF_VIDEO_FORMAT_H264:
        SMF_FFMPEG_LOG_INFO( "ffmpegVideoDecoder " << SMF_ADDR_PREFIX(this) << " open H264 codec" );
        m_pAVCodec = avcodec_find_decoder(AV_CODEC_ID_H264);        
        break;        
    default:
        SMF_FFMPEG_THROW(UnsupportFormatException, "Unknown video format: " <<m_InputType.subType);
        break;
    }

    m_pAVCodecContext = avcodec_alloc_context3(m_pAVCodec);

/*
    av_dict_set(&m_AVOpts,"threads", "1", 0);
    av_dict_set(&m_AVOpts, "threads", "auto", 0);
    av_dict_set(&m_AVOpts, "refcounted_frames", "1", 0);
*/
    if(avcodec_open2(m_pAVCodecContext, m_pAVCodec, &m_AVOpts))
    {
        SMF_FFMPEG_THROW(InternalErrorException, "Cannot open ffmpeg codec" );
    }

    if(enableHWAccel && m_InputType.subType == SMF_VIDEO_FORMAT_H264)
    {        
        createHWAccel();
    }

    m_pAVFrame = av_frame_alloc();
    av_init_packet(&m_Packet);

    m_pAVFrameScaleOut = av_frame_alloc();

    m_InputStatus = ST_ACCEPT_DATA;

    m_FirstPacket = true;        
}

void ffmpegVideoDecoder::destroyCodec(void)
{
    av_dict_free(&m_AVOpts);

    av_free(m_pAVFrame);
    m_pAVFrame = nullptr;

    av_free(m_pAVFrameScaleOut);
    m_pAVFrameScaleOut = nullptr;

    avcodec_close(m_pAVCodecContext);

    if(m_pHWAccel)
    {
        ffmpegManager::getInstancePtr()->destroyHWAccel(m_pAVCodecContext);
        m_pHWAccel = nullptr;
    }

    avcodec_free_context(&m_pAVCodecContext);

    sws_freeContext(m_pSwsContext);

    m_InputStatus = ST_NOT_ACCEPT_DATA;
}

void ffmpegVideoDecoder::createHWAccel(void)
{
    m_pHWAccel = ffmpegManager::getInstancePtr()->createHWAccel(m_pAVCodecContext, m_pDeviceManager);

    if(m_pHWAccel)
    {
        SMF_FFMPEG_LOG_INFO( "ffmpegVideoDecoder " << SMF_ADDR_PREFIX(this) << " Enable hardware accelerator" );

        mf::WindowHandle winId = m_Attributes.getAttribuate<mf::WindowHandle>(ATTRI_VO_TARGET);
        m_pHWAccel->setDirectVideoOutTarget(winId);
    }
}


void ffmpegVideoDecoder::resetStatistics(void)
{
    m_LastFramerateCounter.reset();
    m_AvgFramerateCounter.reset();
    m_BitrateCounter.reset();
    m_Statistics.reset();
    m_Clock.restart();
}

void ffmpegVideoDecoder::updateStatistics(void)
{
    int64 current = m_Clock.getMilliSeconds();

    if(m_LastFramerateCounter.update(current))
    {
        m_Statistics.lastFPS = m_LastFramerateCounter.getFPS();
        //SMF_FFMPEG_LOG_DEBUG("fps = " << m_LastFramerateCounter.getFPS());
    }

    if(m_AvgFramerateCounter.update(current))
    {
        m_Statistics.avgFPS = m_AvgFramerateCounter.getFPS();
    }

    if(m_BitrateCounter.update(current))
    {
        m_Statistics.bitrate = m_BitrateCounter.getBitrate();
        //SMF_FFMPEG_LOG_DEBUG("bitrate = " << m_BitrateCounter.getBitrate());
    }
}

void ffmpegVideoDecoder::updateStreamInfo(void)
{
    m_StreamInfo.width = m_pAVCodecContext->coded_width;
    m_StreamInfo.height = m_pAVCodecContext->coded_height;
    m_StreamInfo.mediaType.majoryType = SMF_MT_VIDEO;

    if(m_pHWAccel)
        m_StreamInfo.mediaType.subType = m_pHWAccel->getFormat();
    else
        m_StreamInfo.mediaType.subType = ffmpegMappings::getMediaSubType(m_pAVCodecContext->pix_fmt);

    const char* profile = av_get_profile_name(m_pAVCodec, m_pAVCodecContext->profile);

    SMF_FFMPEG_LOG_INFO("Profile " << profile);
}

} // namespace mf
} // namespace surveon


