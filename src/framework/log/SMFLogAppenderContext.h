#ifndef SMF_APPENDER_IMPL_H
#define SMF_APPENDER_IMPL_H

#include "SMFLogImplPrerequisites.h"
#include "framework/log/SMFILogAppender.h"


namespace surveon
{
namespace mf
{
namespace log
{

class AppenderContext
{
public:
    typedef log4cplus::Appender* ContextPointeeType;
    typedef log4cplus::SharedAppenderPtr ContextType;

public:
    AppenderContext(void)
    {

    }

    AppenderContext(ContextPointeeType ctx) : m_pLog4cplusAppender(ctx)
    {

    }

   ContextType getContext(void)
    {
        return m_pLog4cplusAppender;
    }

protected:
    ContextType m_pLog4cplusAppender;
};



} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_FILE_APPENDER_H



