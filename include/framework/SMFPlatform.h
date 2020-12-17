/*
*/
#ifndef SMF_PLATFORM_H
#define SMF_PLATFORM_H

#include <string>
#include <sstream>

//==============================================================================
// Identify the operating system
// see http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
//==============================================================================

#if defined(_WIN32)

    // Windows
    #define SMF_PLATFORM_WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(__unix__)

    // UNIX system, see which one it is
    #if  defined(__linux__)

         // Linux
        #define SMF_PLATFORM_LINUX

    #else

        // Unsupported UNIX system
        #error This UNIX operating system is not supported by SMF

    #endif

#else

    // Unsupported system
    #error This operating system is not supported by SMF

#endif


//==============================================================================
// Define helpers to create portable import / export macros for each module
//==============================================================================
#if !defined(SMF_STATIC)

    #if defined(SMF_PLATFORM_WIN32)

        // Windows compilers need specific (and different) keywords for export and import
        #define SMF_API_EXPORT __declspec(dllexport)
        #define SMF_API_IMPORT __declspec(dllimport)

        // For Visual C++ compilers, we also need to turn off this annoying C4251 warning
        #ifdef _MSC_VER

            #pragma warning(disable: 4251)

        #endif

    #else // Linux, FreeBSD, Mac OS X

        #if __GNUC__ >= 4

            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
            #define SMF_API_EXPORT __attribute__ ((__visibility__ ("default")))
            #define SMF_API_IMPORT __attribute__ ((__visibility__ ("default")))

        #else

            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define SMF_API_EXPORT
            #define SMF_API_IMPORT

        #endif

    #endif

#else

    // Static build doesn't need import/export macros
    #define SMF_API_EXPORT
    #define SMF_API_IMPORT

#endif

namespace surveon
{
namespace mf
{

//===============================================================
// Define portable fixed-size Integer types
//===============================================================

// 8 bits integer types
typedef signed char int8;
typedef unsigned char uint8;

// 16 bits integer types
typedef signed short int16;
typedef unsigned short uint16;

// 32 bits integer types
typedef signed int int32;
typedef unsigned int uint32;
typedef unsigned int uint;


// 64 bits integer types
#if defined(_MSC_VER)
    typedef signed   __int64 int64;
    typedef unsigned __int64 uint64;
#else
    typedef signed long long int64;
    typedef unsigned long long uint64;
#endif

typedef int64 time_value;
typedef time_value milli_seconds;
typedef time_value micro_seconds;
typedef time_value nano_seconds;


typedef std::string String;
typedef std::stringstream StringStream;


#ifdef SMF_PLATFORM_WIN32

struct HWND__;
typedef HWND__* WindowHandle;
#define SMF_NO_EXCEPT

#elif defined(SMF_PLATFORM_LINUX)
typedef unsigned long WindowHandle;
#define SMF_NO_EXCEPT noexcept
#else
typedef void* WindowHandle;
#endif


} // namespace mf
} // namespace surveon

#endif // SMF_PLATFORM_H

