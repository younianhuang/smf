#ifndef SMF_DYNAMIC_LIBRARY_H
#define SMF_DYNAMIC_LIBRARY_H

#include "SMFCorePrerequisites.h"


#ifdef SMF_PLATFORM_WIN32
#define SMF_DYNLIB_HANDLE hInstance

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#else
#define SMF_DYNLIB_HANDLE void*

#endif


namespace surveon
{
namespace mf
{

class DynamicLibrary
{
public:
    DynamicLibrary(const String& name);
    ~DynamicLibrary();

    /** Load the library
            */
    void load();
    /** Unload the library
    */
    void unload();
    /// Get the name of the library
    const String& getName(void) const { return m_Name; }

    /**
        Returns the address of the given symbol from the loaded library.
        @param
            strName The name of the symbol to search for
        @return
            If the function succeeds, the returned value is a handle to
            the symbol.
        @par
            If the function fails, the returned value is <b>NULL</b>.
    */
     void* getSymbol( const String& name ) const;

private:
    String m_Name;

    SMF_DYNLIB_HANDLE m_Handle;
};

} // namespace mf
} // namespace surveon

#endif // SMF_DYNAMIC_LIBRARY_H


