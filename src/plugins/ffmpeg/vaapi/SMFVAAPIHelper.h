#ifndef SMF_VAAPI_HELPER_H
#define SMF_VAAPI_HELPER_H

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "libavcodec/vaapi.h"

#include "libavutil/avassert.h"
#include "libavutil/buffer.h"
#include "libavutil/frame.h"
#include "libavutil/pixfmt.h"

#include <va/va.h>
#include <va/va_x11.h>

}

#include "../SMFffmpegException.h"



#define SMF_VAAPI_CHECK(vaapifun)   \
        { \
            SMF_FFMPEG_LOG_TRACE(#vaapifun); \
            VAStatus status = vaapifun;  \
            if(status != VA_STATUS_SUCCESS) \
            {   \
                SMF_FFMPEG_THROW(InternalErrorException, \
                "Fail to call " << #vaapifun << " : " << vaErrorStr(status)); \
            }  \
        }

#define SMF_VAAPI_TRACE(vaapifun)   \
        { \
            SMF_FFMPEG_LOG_TRACE(#vaapifun); \
            VAStatus status = vaapifun;  \
            if(status != VA_STATUS_SUCCESS) \
            {   \
                SMF_FFMPEG_LOG_WARN(\
                "Fail to call " << #vaapifun << " : " << vaErrorStr(status)); \
            }  \
        }


#endif // SMF_VAAPI_HELPER_H
