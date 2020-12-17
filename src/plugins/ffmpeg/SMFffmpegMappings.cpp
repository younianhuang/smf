#include "SMFffmpegMappings.h"

namespace surveon
{
namespace mf
{

MediaSubType ffmpegMappings::getMediaSubType(AVPixelFormat format)
{
    switch(format)
    {
    case AV_PIX_FMT_YUV420P:
    case AV_PIX_FMT_YUVJ420P:
        return SMF_VIDEO_FORMAT_YUV420;

    case AV_PIX_FMT_RGBA:
        return SMF_VIDEO_FORMAT_RGBA;

    case AV_PIX_FMT_VDPAU:
        return SMF_VIDEO_FORMAT_VDPAU;

    case AV_PIX_FMT_VAAPI_VLD:
        return SMF_VIDEO_FORMAT_VAAPI;
    }

    assert(0 && "unknown format");
    return SMF_SUB_TYPE_NONE;
}


} // namespace mf
} // namespace surveon

