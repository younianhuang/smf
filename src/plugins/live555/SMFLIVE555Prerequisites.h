
#ifndef SMF_LIVE555_PREREQUISITES_H
#define SMF_LIVE555_PREREQUISITES_H

#include "framework/SMFUtil.h"
#include "framework/SMFLog.h"
#include "framework/SMFException.h"
#include "framework/SMFPrimitives.h"
#include <framework/SMFPipeline.h>
#include <framework/pipeline/SMFIStreamSource.h>

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

#include "Base64.h"
#include "SMFAVIFileSink.hh"
#include "SMFRTSPVideoStreamSource.h"
#include "SMFRTSPSource.h"

#include <string.h>
#include <assert.h>
#include <iostream>

////////////////////////////////////////////////////////////
// Define portable import / export macros
////////////////////////////////////////////////////////////
#if defined(SMF_LIVE555_EXPORTS)

    #define SMF_LIVE555_API   SMF_API_EXPORT
    #define SMF_LIVE555_TEMPLATE_INSTANTIATE

#else

    #define SMF_LIVE555_API SMF_API_IMPORT
    #define SMF_LIVE555_INSTANTIATE  extern

#endif


////////////////////////////////////////////////////////////
// Warning flags
////////////////////////////////////////////////////////////
#ifdef SMF_PLATFORM_LINUX
    #pragma GCC diagnostic ignored "-Wunused-function"
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-result"
    #pragma GCC diagnostic ignored "-Wsign-compare"
    #pragma GCC diagnostic ignored "-Wreorder"
    #pragma GCC diagnostic ignored "-Wparentheses"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #pragma GCC diagnostic ignored "-Wempty-body"
    #pragma GCC diagnostic ignored "-Wwrite-strings"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#else
    #pragma warning(disable: 4390) // -Wempty-body
#endif


#endif // SMF_LIVE555_PREREQUISITES_H

