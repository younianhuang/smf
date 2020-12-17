
#include "SMFAVIVideoStreamSource.h"


namespace surveon
{
namespace mf
{

AVIVideoStreamSource::AVIVideoStreamSource(int index, AVCodecContext* pCodecContext) :
    m_Active(true),
    m_Index(index),
    m_pCodecContext(pCodecContext)

{
    assert(pCodecContext);
    m_MediaType.majoryType = SMF_MT_VIDEO;

    switch(m_pCodecContext->codec_id)
    {
    case AV_CODEC_ID_MJPEG:
        m_MediaType.subType = SMF_VIDEO_FORMAT_MJPEG;
        break;

    case AV_CODEC_ID_MPEG4:
        m_MediaType.subType = SMF_VIDEO_FORMAT_MPEG4;
        break;

    case AV_CODEC_ID_H264:
        m_MediaType.subType = SMF_VIDEO_FORMAT_H264;
        break;

    default:
        SMF_FFMPEG_THROW(UnsupportFormatException, "Unsupport codec " << m_pCodecContext->codec_id);
    }
}

AVIVideoStreamSource::~AVIVideoStreamSource()
{

}

const MediaType& AVIVideoStreamSource::getMediaType(void) const
{
    return m_MediaType;
}

void AVIVideoStreamSource::setActive(bool active)
{
    m_Active = active;
}

bool AVIVideoStreamSource::isActive(void) const
{
    return m_Active;
}

int AVIVideoStreamSource::getStreamIndex(void) const
{
    return m_Index;
}

void AVIVideoStreamSource::getStatistics(Statistics& stat) const
{
    stat = m_Statistics;
}

void AVIVideoStreamSource::resetStatistics(void)
{
    m_LastFramerateCounter.reset();
    m_AvgFramerateCounter.reset();
    m_BitrateCounter.reset();
    m_Statistics.reset();
    m_Clock.restart();
}

void AVIVideoStreamSource::updateStatistics(uint64 dataSizeInBits)
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

    m_BitrateCounter.accumulate(dataSizeInBits);

    if(m_BitrateCounter.update(current))
    {
        m_Statistics.bitrate = m_BitrateCounter.getBitrate();
        //SMF_FFMPEG_LOG_DEBUG("bitrate = " << m_BitrateCounter.getBitrate());
    }
}

} // namespace mf
} // namespace surveon


