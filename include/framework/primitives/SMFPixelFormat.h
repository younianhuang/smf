#ifndef SMF_PIXEL_FORMAT_H
#define SMF_PIXEL_FORMAT_H

#include "framework/SMFPrerequisites.h"
#include "framework/primitives/SMFMediaType.h"

namespace surveon
{
namespace mf
{

struct SMF_API PlaneFormatDescription
{
    /// down sampling of width
    size_t widthDiv;
    /// down sampling of height
    size_t heightDiv;
};

struct SMF_API PixelFormatDescription
{
    MediaSubType pixelFormat;
    size_t bitsPerPixel;
    std::vector<PlaneFormatDescription> planeFormatDescriptions;
};


class SMF_API PixelFormatDescriptor
{
public:
    static const PixelFormatDescription& getPixelFormatDescription(MediaSubType pixelFormat);

};


template<int format>
struct PFDef
{
    enum
    {
        BITS_PER_PIXEL = -1,
        NUM_OF_PLANES = -1
    };
};


template<> struct PFDef<SMF_VIDEO_FORMAT_RGBA>
{
    enum
    {
        BITS_PER_PIXEL = 32,
        NUM_OF_PLANES = 1,
        PLANE1_WIDTH_DIV =1,
        PLANE1_HEIGHT_DIV =1
    };
};

template<> struct PFDef<SMF_VIDEO_FORMAT_YUV420>
{
    enum
    {
        BITS_PER_PIXEL = 8,
        NUM_OF_PLANES = 3,
        PLANE1_WIDTH_DIV =1,
        PLANE1_HEIGHT_DIV =1,
        PLANE2_WIDTH_DIV =2,
        PLANE2_HEIGHT_DIV =2,
        PLANE3_WIDTH_DIV =2,
        PLANE3_HEIGHT_DIV =2,
    };
};

template<> struct PFDef<SMF_VIDEO_FORMAT_YV12>
{
    enum
    {
        BITS_PER_PIXEL = 8,
        NUM_OF_PLANES = 3,
        PLANE1_WIDTH_DIV =1,
        PLANE1_HEIGHT_DIV =1,
        PLANE2_WIDTH_DIV =2,
        PLANE2_HEIGHT_DIV =2,
        PLANE3_WIDTH_DIV =2,
        PLANE3_HEIGHT_DIV =2,
    };
};




} // namespace mf
} // namespace surveon

#endif // SMF_PIXEL_FORMAT_H

