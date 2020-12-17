#include "SMFffmpegCodec.h"

namespace surveon
{
namespace mf
{

ffmpegCodec::ffmpegCodec(MediaSubType type) :
    m_pAVCodecContext(nullptr),
    m_pAVCodec(nullptr),
    m_pAVFrame(nullptr)
{
    switch(type)
    {
    case SMF_VIDEO_FORMAT_MJPEG:
        m_pAVCodec = avcodec_find_decoder(AV_CODEC_ID_MJPEG);
        break;
    case SMF_VIDEO_FORMAT_MPEG4:
        m_pAVCodec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
        break;
    case SMF_VIDEO_FORMAT_H264:
        m_pAVCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
        break;
    }

    if(!m_pAVCodec)
    {
        SMF_FFMPEG_THROW(UnsupportFormatException, "Unsupport video format: " << type);
    }

    m_pAVCodecContext = avcodec_alloc_context3(m_pAVCodec);

    if(avcodec_open2(m_pAVCodecContext, m_pAVCodec, nullptr))
    {
        SMF_FFMPEG_THROW(InternalErrorException, "Cannot open ffmpeg codec" );
    }

    m_pAVFrame = av_frame_alloc();
}

ffmpegCodec::~ffmpegCodec()
{
    av_free(m_pAVFrame);

    avcodec_close(m_pAVCodecContext);
}


} // namespace mf
} // namespace surveon



