#include "SMFDynamicLibrary.h"

#ifdef SMF_PLATFORM_WIN32

#include <windows.h>

#    define SMF_DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define SMF_DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define SMF_DYNLIB_UNLOAD( a ) !FreeLibrary( a )

#else

#include <dlfcn.h>

#    define SMF_DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define SMF_DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define SMF_DYNLIB_UNLOAD( a ) dlclose( a )

#endif


namespace surveon
{
namespace mf
{

DynamicLibrary::DynamicLibrary(const String& name)  :
    m_Name(name),
    m_Handle(nullptr)
{

}

DynamicLibrary::~DynamicLibrary()
{
    unload();
}

void DynamicLibrary::load()
{
    SMF_CORE_LOG_INFO("Load library " << m_Name);

    String name;

#ifdef SMF_PLATFORM_WIN32
    name = m_Name;
#else

    if(m_Name.find("lib") == String::npos)
        name = "lib" + m_Name;
    else
        name = m_Name;

    if (name.find(".so") == String::npos)
        name += ".so";

#endif

    m_Handle = SMF_DYNLIB_LOAD( name.c_str() );

    if(!m_Handle)
    {        
        SMF_CORE_THROW_EXCEPT(InternalErrorException, "Fail to load library " << m_Name);
    }
}

void DynamicLibrary::unload()
{
    SMF_CORE_LOG_INFO("Unload library " << m_Name);

    if( SMF_DYNLIB_UNLOAD( m_Handle ) )
    {
        SMF_CORE_LOG_ERROR("Fail to unload library " << m_Name);
    }
}

void* DynamicLibrary::getSymbol( const String& name ) const
{
    return (void*)SMF_DYNLIB_GETSYM( m_Handle, name.c_str() );
}

} // namespace mf
} // namespace surveon
