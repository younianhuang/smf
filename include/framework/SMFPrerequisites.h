
#ifndef SMF_PREREQUISITES_H
#define SMF_PREREQUISITES_H

#include "SMFPlatform.h"
#include "SMFConfig.h"

// C library headers
#include <cstdlib>
#include <assert.h>

// C++ headers
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <list>
#include <memory>
#include <functional>
#include <cstring>


//==============================================================================
// Define the SMF version
//==============================================================================
#define SMF_VERSION_MAJOR 0
#define  SMF_VERSION_MINOR 2
#define  SMF_VERSION_PATCH 0



////////////////////////////////////////////////////////////
// Define portable import / export macros
////////////////////////////////////////////////////////////
#if defined(SMF_EXPORTS)

    #define SMF_API SMF_API_EXPORT
    #define SMF_TEMPLATE_INSTANTIATE

#else

    #define SMF_API SMF_API_IMPORT
    #define SMF_TEMPLATE_INSTANTIATE extern

#endif

#endif // SMF_PREREQUISITES_H

