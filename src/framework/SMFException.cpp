
#include "framework/SMFException.h"

namespace surveon
{
namespace mf
{

    Exception::Exception(const String& typeName,
                  const String& description,
                  const String&file,
                  int line,
                  const String& function) :
        m_Line(line),
        m_TypeName(typeName),
        m_Description(description),
        m_File(file),
        m_Function(function)
    {

    }

    Exception::~Exception(void)
    {

    }

    const String& Exception::getDescription(void) const
    {
        return m_Description;
    }

    const String& Exception::getLogDescription(void) const
    {
        if(m_LogDescription.empty())
        {
            StringStream ss;

            ss << m_TypeName << " : "
                << m_Description
                << " throw by ";

            m_LogDescription = ss.str();
        }
        return m_LogDescription;
    }

    const String& Exception::getFullDescription(void) const
    {
        if (m_FullDescription.empty())
        {
            StringStream ss;

            ss << m_TypeName << " : "
                << m_Description
                << " in " << m_Function;

            if( m_Line > 0 )
            {
                ss << " at " << m_File << " (line " << m_Line << ")";
            }

            m_FullDescription = ss.str();
        }

        return m_FullDescription;
    }
#ifdef __linux__
    const char*Exception:: what(void) const noexcept
#else // #elif _WIN32
    const char*Exception:: what(void) const
#endif
    {
        return getFullDescription().c_str();
    }

} // namespace mf
} // namespace surveon


