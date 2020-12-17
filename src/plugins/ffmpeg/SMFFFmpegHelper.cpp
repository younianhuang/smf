#include "SMFFFmpegHelper.h"


namespace surveon
{
namespace mf
{


void ffmpegHelper::initialize(void)
{
    SMF_FFMPEG_LOG_INFO("Initialize ffmpeg");

#ifndef NDEBUG
    av_log_set_level(AV_LOG_DEBUG);
#endif
    av_register_all();

    av_log_set_callback(&ffmpegHelper::logCallback);
}

void ffmpegHelper::shutdown(void)
{

}


String ffmpegHelper::getErrorString(int errnum)
{
    char errorStr[AV_ERROR_MAX_STRING_SIZE] {0};
    return String(av_make_error_string(errorStr, AV_ERROR_MAX_STRING_SIZE, errnum));
}



//========================================================================

void ffmpegHelper::logCallback(void* /*ptr*/, int level, const char* fmt, va_list vl)
{

    char msg[512];
    vsprintf(msg, fmt, vl);

    switch(level)
    {
    case AV_LOG_PANIC:
    case AV_LOG_FATAL:
        SMF_FFMPEG_LOG_FATAL("ffmpeg:" << msg);
        break;

    case AV_LOG_ERROR:
        SMF_FFMPEG_LOG_ERROR("ffmpeg:"<< msg);
        break;

    case AV_LOG_WARNING:
        SMF_FFMPEG_LOG_WARN("ffmpeg:"<< msg);
        break;

    case AV_LOG_INFO:
        SMF_FFMPEG_LOG_INFO("ffmpeg:"<< msg);
        break;

    case AV_LOG_VERBOSE:
    case AV_LOG_DEBUG:
        SMF_FFMPEG_LOG_DEBUG("ffmpeg:"<< msg);
        break;

    case AV_LOG_TRACE:
        SMF_FFMPEG_LOG_TRACE("ffmpeg:"<< msg);
        break;
}





}

} // namespace mf
} // namespace surveon
