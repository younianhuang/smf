

#include "framework/primitives/SMFPixelFormat.h"


namespace surveon
{
namespace mf
{


const PixelFormatDescription& PixelFormatDescriptor::getPixelFormatDescription(MediaSubType pixelFormat)
{
    static const PixelFormatDescription s_PixelFormatDescriptions[] =
    {
        {
            SMF_VIDEO_FORMAT_RGBA, PFDef<SMF_VIDEO_FORMAT_RGBA>::BITS_PER_PIXEL,
            {
              {PFDef<SMF_VIDEO_FORMAT_RGBA>::PLANE1_WIDTH_DIV,   PFDef<SMF_VIDEO_FORMAT_RGBA>::PLANE1_HEIGHT_DIV}
            }
        },
        {
            SMF_VIDEO_FORMAT_YUV420, PFDef<SMF_VIDEO_FORMAT_YUV420>::BITS_PER_PIXEL,
           {
                  {PFDef<SMF_VIDEO_FORMAT_YUV420>::PLANE1_WIDTH_DIV,    PFDef<SMF_VIDEO_FORMAT_YUV420>::PLANE1_HEIGHT_DIV},
                  {PFDef<SMF_VIDEO_FORMAT_YUV420>::PLANE2_WIDTH_DIV,    PFDef<SMF_VIDEO_FORMAT_YUV420>::PLANE2_HEIGHT_DIV},
                  {PFDef<SMF_VIDEO_FORMAT_YUV420>::PLANE3_WIDTH_DIV,    PFDef<SMF_VIDEO_FORMAT_YUV420>::PLANE3_HEIGHT_DIV}
            }
        },
        {
            SMF_VIDEO_FORMAT_YV12, PFDef<SMF_VIDEO_FORMAT_YV12>::BITS_PER_PIXEL,
           {
                  {PFDef<SMF_VIDEO_FORMAT_YV12>::PLANE1_WIDTH_DIV,    PFDef<SMF_VIDEO_FORMAT_YV12>::PLANE1_HEIGHT_DIV},
                  {PFDef<SMF_VIDEO_FORMAT_YV12>::PLANE2_WIDTH_DIV,    PFDef<SMF_VIDEO_FORMAT_YV12>::PLANE2_HEIGHT_DIV},
                  {PFDef<SMF_VIDEO_FORMAT_YV12>::PLANE3_WIDTH_DIV,    PFDef<SMF_VIDEO_FORMAT_YV12>::PLANE3_HEIGHT_DIV}
            }
        }
    };

    assert( pixelFormat >= SMF_VIDEO_FORMAT_RGBA);
    assert( pixelFormat <= SMF_VIDEO_FORMAT_YV12);

    return s_PixelFormatDescriptions[pixelFormat-SMF_VIDEO_FORMAT_RGBA];
}


} // namespace mf
} // namespace surveon


