/*
*/
#ifndef GLVR_CONFIG_H
#define GLVR_CONFIG_H


//==============================================================================
// Identify the operating system
// see http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
//==============================================================================

#if defined(_WIN32)

    // Windows
    #define GLVR_SYSTEM_WINDOWS
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(__unix__)

    // UNIX system, see which one it is
    #if  defined(__linux__)

         // Linux
        #define GLVR_SYSTEM_LINUX

    #else

        // Unsupported UNIX system
        #error This UNIX operating system is not supported by GLVR

    #endif

#else

    // Unsupported system
    #error This operating system is not supported by GLVR

#endif


//==============================================================================
// Define helpers to create portable import / export macros for each module
//==============================================================================
#if !defined(GLVR_STATIC)

    #if defined(GLVR_SYSTEM_WINDOWS)

        // Windows compilers need specific (and different) keywords for export and import
        #define GLVR_API_EXPORT __declspec(dllexport)
        #define GLVR_API_IMPORT __declspec(dllimport)

        // For Visual C++ compilers, we also need to turn off this annoying C4251 warning
        #ifdef _MSC_VER

            #pragma warning(disable: 4251)

        #endif

    #else // Linux, FreeBSD, Mac OS X

        #if __GNUC__ >= 4

            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
            #define GLVR_API_EXPORT __attribute__ ((__visibility__ ("default")))
            #define GLVR_API_IMPORT __attribute__ ((__visibility__ ("default")))

        #else

            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define GLVR_API_EXPORT
            #define GLVR_API_IMPORT

        #endif

    #endif

#else

    // Static build doesn't need import/export macros
    #define GLVR_API_EXPORT
    #define GLVR_API_IMPORT

#endif


#endif // GLVR_CONFIG_H

