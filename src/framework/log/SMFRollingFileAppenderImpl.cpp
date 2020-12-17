#include "SMFRollingFileAppenderImpl.h"

namespace surveon
{
namespace mf
{
namespace log
{


//==================================================================================================

RollingFileAppenderImpl::RollingFileAppenderImpl(const String& filename) :
    m_AppenderContext(new log4cplus::RollingFileAppender(filename))
{
    std::auto_ptr<log4cplus::Layout> pLayout(
                new log4cplus::PatternLayout(
                    LOG4CPLUS_TEXT("%D{%Y-%m-%d-%H:%M:%S.%q} [%8t] %8c %5p -  %m [%M() %b:%L] %n"
                    )));

    m_AppenderContext.getContext()->setLayout(pLayout);
}

RollingFileAppenderImpl::~RollingFileAppenderImpl(void)
{

}

AppenderContext* RollingFileAppenderImpl::getContext(void)
{
    return &m_AppenderContext;
}

} // namespace log
} // namespace mf
} // namespace surveon



