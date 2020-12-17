
#ifndef SMF_FFMPEG_HELPER_H
#define SMF_FFMPEG_HELPER_H

#include "SMFffmpegPrerequisites.h"
#include "SMFffmpegLog.h"


extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


namespace surveon
{
namespace mf
{

/**
 */
class ffmpegHelper
{

public:
    static void initialize(void);
    static void shutdown(void);

    static void logCallback(void* ptr, int level, const char* fmt, va_list vl);
    static String getErrorString(int errnum);


};

} // namespace mf
} // namespace surveon



#endif // SMF_FFMPEG_HELPER_H
