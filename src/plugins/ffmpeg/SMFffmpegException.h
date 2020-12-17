#ifndef SMF_FFMPEG_EXCEPTION_H
#define SMF_FFMPEG_EXCEPTION_H

#include "SMFffmpegLog.h"


#define SMF_FFMPEG_THROW(ExceptType, desc)  { \
        surveon::mf::StringStream ss;   \
        ss << desc;  \
        ExceptType ex(ss.str(), __FILE__, __LINE__, __FUNCTION__);   \
        SMF_FFMPEG_LOG_ERROR(ex.getLogDescription());  \
        throw ex; }


#endif // SMF_FFMPEG_EXCEPTION_H

