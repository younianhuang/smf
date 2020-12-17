
#ifndef SMF_MEDIA_TYPE_H
#define SMF_MEDIA_TYPE_H


#include "framework/SMFPrerequisites.h"


namespace surveon
{
namespace mf
{

enum MediaMajorType
{
    SMF_MAJOR_TYPE_NONE,
    SMF_MT_VIDEO,
    SMF_MT_AUDIO,
    SMF_MAJOR_TYPE_END
};

enum MediaSubType
{
    SMF_SUB_TYPE_NONE,
    SMF_VIDEO_FORMAT_MJPEG,
    SMF_VIDEO_FORMAT_MPEG4,
    SMF_VIDEO_FORMAT_H264,
    SMF_VIDEO_FORMAT_RGBA,    
    SMF_VIDEO_FORMAT_YUV420,
    SMF_VIDEO_FORMAT_YV12,
    SMF_VIDEO_FORMAT_VDPAU,
    SMF_VIDEO_FORMAT_VAAPI,
    SMF_VIDEO_FORMAT_DXVA2,
    SMF_VIDEO_FORMAT_XPIXMAP,
    SMF_VIDEO_FORMAT_D3D9_SURFACE,
    SMF_SUB_TYPE_END
};


struct SMF_API MediaType
{
    MediaMajorType majoryType = {SMF_MAJOR_TYPE_NONE};
    MediaSubType subType= {SMF_SUB_TYPE_NONE};
};

} // namespace mf
} // namespace surveon



#endif // SMF_MEDIA_TYPE_H


