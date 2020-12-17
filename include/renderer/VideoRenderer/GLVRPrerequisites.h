#ifndef GLVR_PREREQUISITES_H
#define GLVR_PREREQUISITES_H

#include "GLVRConfig.h"

// C++ std library headers
#include <cmath>

// SMF library headers
#include <framework/SMFPrerequisites.h>
#include <framework/SMFPrimitives.h>
#include <framework/SMFUtil.h>
#include <framework/SMFThread.h>
#include <framework/SMFPipeline.h>

//==============================================================================
// Define the GLVR version
//==============================================================================
#define GLVR_VERSION_MAJOR 0
#define  GLVR_VERSION_MINOR 2
#define  GLVR_VERSION_PATCH 0



//==============================================================================
// Define portable import / export macros
//==============================================================================
#if defined(GLVR_EXPORTS)

    #define GLVR_API GLVR_API_EXPORT
    #define GLVR_TEMPLATE_INSTANTIATE

#else

    #define GLVR_API GLVR_API_IMPORT
    #define GLVR_TEMPLATE_INSTANTIATE extern

#endif


namespace surveon
{

namespace glvr
{

using namespace surveon::mf;


} // namespace glvr
} // namespace surveon


#endif // GLVR_PREREQUISITES_H

