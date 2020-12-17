#ifndef SMF_FFMPEG_MAPPINGS_H
#define SMF_FFMPEG_MAPPINGS_H

#include "SMFFFmpegHelper.h"

namespace surveon
{
namespace mf
{

/**
 */
class ffmpegMappings
{
public:
    ffmpegMappings();
    ~ffmpegMappings();

    static MediaSubType getMediaSubType(AVPixelFormat format);
};


} // namespace mf
} // namespace surveon



#endif // SMF_FFMPEG_MAPPINGS_H
