#include "SMFAVIFileReader.h"
#include "SMFFFmpegHelper.h"
#include "SMFffmpegLog.h"

namespace surveon
{
namespace mf
{

const size_t BUFFER_NUM = 10;
const size_t BUFFER_SIZE = 10 * 1024;


MediaBufferRingPool::MediaBufferRingPool(size_t numOfBuffers, size_t bufferSize) :
    m_MediaBuffers(numOfBuffers),
    m_ReadIndex(0),
    m_WriteIndex(0)
{
    IMediaBufferFactory* pFactory =  mf::getSystemMemoryMediaBufferFactory();

    for(size_t i=0; i<numOfBuffers; i++)
    {
        IMediaBuffer* pMediaBuffer = pFactory->createMediaBuffer(bufferSize);
        m_MediaBuffers[i] = pMediaBuffer;
    }
}

IMediaBuffer* MediaBufferRingPool::getToRead(void)
{
    if(isEmpty())
        return nullptr;

    IMediaBuffer* pMediaBuffer = m_MediaBuffers[m_ReadIndex];
    if(++m_ReadIndex >= m_MediaBuffers.size())
    {
        m_ReadIndex = 0;
    }

    return pMediaBuffer;
}

IMediaBuffer* MediaBufferRingPool::getToWrite(size_t size)
{
    IMediaBuffer* &pMediaBuffer = m_MediaBuffers[m_WriteIndex];

    if(pMediaBuffer->getMaxLength() < size + FF_INPUT_BUFFER_PADDING_SIZE)
    {
        IMediaBufferFactory* pFactory =  mf::getSystemMemoryMediaBufferFactory();
        pFactory->destroyMediaBuffer(pMediaBuffer);
        pMediaBuffer = pFactory->createMediaBuffer(size + FF_INPUT_BUFFER_PADDING_SIZE);
    }

    if(++m_WriteIndex >= m_MediaBuffers.size())
    {
        m_WriteIndex = 0;
    }

    return pMediaBuffer;
}


void MediaBufferRingPool::reset(void)
{
    m_ReadIndex = 0;
    m_WriteIndex = 0;
}


//===============================================================================

AVIFileReader::AVIFileReader(mf::AsyncCommandQueue* pCommandQueue) :
    m_pFormatContext (nullptr),
    m_Initialized(false),
    m_pCommandQueue(pCommandQueue),
    m_StreamStatus(SS_STOP),
    m_MediaBufferPool(BUFFER_NUM, BUFFER_SIZE),
    m_VideoStreamIndex(-1),
    m_AudioStreamIndex(-1)
{
    SMF_FFMPEG_LOG_INFO( "Create AVIFileReader " << SMF_ADDR_PREFIX(this));
    assert(pCommandQueue);
}

AVIFileReader::~AVIFileReader()
{    
    if(m_Initialized)
    {
        closeCommand();
    }

    SMF_FFMPEG_LOG_INFO( "Destroy AVIFileReader " << SMF_ADDR_PREFIX(this));
}

void AVIFileReader::initialize(const String& filename)
{        
    SMF_FFMPEG_LOG_INFO( "initialize AVIFileReader " << SMF_ADDR_PREFIX(this));

    if(m_Initialized)
    {
        SMF_FFMPEG_THROW(InvalidStateException, "This AVIFileReader is already initialized.");
    }

    m_StreamStatus = SS_STOP;

    m_Initialized = true;

    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_OPEN_FILE,
                &AVIFileReader::openCommand, this, filename);
}

void AVIFileReader::shutdown(void)
{    
    if(m_Initialized)
    {
        SMF_FFMPEG_LOG_INFO( "shutdown AVIFileReader " << SMF_ADDR_PREFIX(this));

        m_pCommandQueue->blockingCall<void>(
                    AsyncCommand::CT_OPEN_FILE,
                    &AVIFileReader::closeCommand, this);

        m_Initialized = false;        
    }

}

void AVIFileReader::start(uint64 position)
{
    m_pCommandQueue->blockingCall<void>(
                AsyncCommand::CT_SET,
                &AVIFileReader::startCommand, this, position);
}

void AVIFileReader::stop(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &AVIFileReader::stopCommand, this);
}

void AVIFileReader::pause(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &AVIFileReader::pauseCommand, this);
}

void AVIFileReader::resume(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &AVIFileReader::resumeCommand, this);
}

AVIFileReader::StreamStatus AVIFileReader::getPacket(IMediaBuffer** ppMediaBuffer)
{
    return m_pCommandQueue->blockingCall<StreamStatus>(
                AsyncCommand::CT_GET,
                &AVIFileReader::getPacketCommand, this, ppMediaBuffer);
}


size_t AVIFileReader::getNumOfStream(void) const
{
    return m_pCommandQueue->blockingCall<size_t>(
                AsyncCommand::CT_GET,
                &StreamSources::size, &m_StreamSources);
}

IStreamSource* AVIFileReader::getStreamSource(size_t index)
{                    
    return m_pCommandQueue->blockingCall<IStreamSource*>(
                AsyncCommand::CT_GET,
                &AVIFileReader::getStreamSourceCommand, this, index);
}

String AVIFileReader::getSource(void) const
{
    return m_pCommandQueue->blockingCall<const String&>(
                AsyncCommand::CT_GET,
                &AVIFileReader::getSourceCommand, this);
}


//======================================================

void AVIFileReader::openCommand(const String& filename)
{
    SMF_FFMPEG_LOG_INFO( "Open file " << filename);

    m_Filename = filename;

    if(avformat_open_input(&m_pFormatContext, m_Filename.c_str(), NULL, NULL)!=0)
    {
        SMF_FFMPEG_THROW(OpenFileErrorException, "fail to open file " << m_Filename);
    }

    if(avformat_find_stream_info(m_pFormatContext, NULL)<0)
    {
        SMF_FFMPEG_THROW(OpenFileErrorException, "fail to find any stream in file " << m_Filename);
    }

    av_dump_format(m_pFormatContext, 0, filename.c_str(), 0);

    for(uint32 i=0; i<m_pFormatContext->nb_streams; i++)
    {
        AVCodecContext *pCodecContex = m_pFormatContext->streams[i]->codec;

        // just read first video stream
        if(pCodecContex->codec_type==AVMEDIA_TYPE_VIDEO && m_VideoStreamIndex == -1)
        {           
           m_VideoStreamIndex = i;

           AVIVideoStreamSource* pVideoStreamSource = new  AVIVideoStreamSource(i, pCodecContex);
           m_StreamSources.push_back(pVideoStreamSource);

        }
        else
        {
            m_StreamSources.push_back(nullptr);
        }

    }

    av_init_packet(&m_Packet);
}

void AVIFileReader::closeCommand(void)
{
    SMF_FFMPEG_LOG_INFO( "Close file " << m_Filename);

    ContainerUtil::destroyAllElements(m_StreamSources);

    // Close the codecs
    for(size_t i=0; i<m_pFormatContext->nb_streams; i++)
    {
          AVCodecContext *pCodecContex = m_pFormatContext->streams[i]->codec;
          avcodec_close(pCodecContex);
    }

    // Close the video file
    avformat_close_input(&m_pFormatContext);

    // Free the packet that was allocated by av_read_frame
    av_free_packet(&m_Packet);
}

void AVIFileReader::caculateTimestamp(IMediaBuffer* pMediaBuffer)
{
    int64 pts = 0;
    if(m_Packet.pts != AV_NOPTS_VALUE)
    {
        pts = m_Packet.pts;
    }
    else if(m_Packet.dts == AV_NOPTS_VALUE)
    {
        pts = m_Packet.dts;
    }

    AVStream* pStream = m_pFormatContext->streams[m_Packet.stream_index];

    micro_seconds timestamp = static_cast<micro_seconds>(
             1000000 * m_Packet.dts * m_Packet.duration * av_q2d(pStream->time_base));

    pMediaBuffer->setTimestamp(timestamp);
}

void AVIFileReader::readPacket(IMediaBuffer* pMediaBuffer)
{
    assert(pMediaBuffer->getMaxLength()>= m_Packet.size);

    uint8* buffer = pMediaBuffer->lock();
    memcpy(buffer, m_Packet.data, m_Packet.size);

    pMediaBuffer->setCurrentLength(m_Packet.size);

    pMediaBuffer->unlock();
}

void AVIFileReader::readPackets(void)
{
    if(av_read_frame(m_pFormatContext, &m_Packet)>=0)
    {        
        if(m_VideoStreamIndex == m_Packet.stream_index)
        {
            IMediaBuffer* pMediaBuffer =  m_MediaBufferPool.getToWrite(m_Packet.size);

            readPacket(pMediaBuffer);

            caculateTimestamp(pMediaBuffer);

            pMediaBuffer->setReferenced(true);

            IStreamSource* pStreamSource = m_StreamSources[m_Packet.stream_index];

            static_cast<AVIVideoStreamSource*>(pStreamSource)->
                    updateStatistics(pMediaBuffer->getCurrentLength()<<3);
        }

        av_free_packet(&m_Packet);
    }
    else
    {
        m_StreamStatus = SS_END_OF_STREAM;
    }
}

void AVIFileReader::startCommand(uint64 position)
{
    SMF_FFMPEG_LOG_INFO( "start at position " << position);

    m_StreamStatus = SS_OK;

    if(!m_pFormatContext)
    {
        SMF_FFMPEG_THROW(InvalidStateException, "File is not open");
    }

    int64_t startTime = m_pFormatContext->start_time + position;

    if(avformat_seek_file(m_pFormatContext, -1, INT64_MIN, startTime, INT64_MAX, 0) < 0)
    {
        SMF_FFMPEG_THROW(InternalErrorException,
                         "fail to seek stream: " << m_VideoStreamIndex << " position " << position);
    }

    m_MediaBufferPool.reset();
}

void AVIFileReader::stopCommand(void)
{
    m_StreamStatus = SS_PAUSE;
}

void AVIFileReader::pauseCommand(void)
{
    m_StreamStatus = SS_PAUSE;
}

void AVIFileReader::resumeCommand(void)
{
    m_StreamStatus = SS_OK;
}

AVIFileReader::StreamStatus AVIFileReader::getPacketCommand(IMediaBuffer** ppMediaBuffer)
{
    if(!m_MediaBufferPool.isEmpty())
    {
        *ppMediaBuffer = m_MediaBufferPool.getToRead();
        return SS_OK;
    }
    else
    {
        ppMediaBuffer = nullptr;
        return m_StreamStatus;
    }
}

const String& AVIFileReader::getSourceCommand(void) const
{
    return m_Filename;
}

IStreamSource* AVIFileReader::getStreamSourceCommand(size_t index)
{
    if(index>= m_StreamSources.size())
    {
        SMF_FFMPEG_THROW(InvalidParametersException, "StreamSource " << index << " does not exist!");
    }

    return m_StreamSources[index];
}



} // namespace mf
} // namespace surveon



