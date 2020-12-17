
#ifndef SMF_FFMPEG_PREREQUISITES_H
#define SMF_FFMPEG_PREREQUISITES_H

#include "framework/SMFUtil.h"
#include "framework/SMFLog.h"
#include "framework/SMFException.h"
#include "framework/SMFPrimitives.h"
#include <framework/SMFPipeline.h>


////////////////////////////////////////////////////////////
// Define portable import / export macros
////////////////////////////////////////////////////////////
#if defined(SMF_FFMPEG_EXPORTS)

    #define SMF_FFMPEG_API   SMF_API_EXPORT
    #define SMF_FFMPEG_TEMPLATE_INSTANTIATE

#else

    #define SMF_FFMPEG_API SMF_API_IMPORT
    #define SMF_FFMPEG_INSTANTIATE  extern

#endif

#endif // SMF_FFMPEG_PREREQUISITES_H

