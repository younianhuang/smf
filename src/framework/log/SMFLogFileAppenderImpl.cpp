
#include "SMFLogFileAppenderImpl.h"

namespace surveon
{
namespace mf
{
namespace log
{


//==================================================================================================

FileAppenderImpl::FileAppenderImpl(const String& filename) :
    m_AppenderContext(new log4cplus::FileAppender(filename))
{
    std::auto_ptr<log4cplus::Layout> pLayout(
                new log4cplus::PatternLayout(
                    LOG4CPLUS_TEXT("%D{%Y-%m-%d-%H:%M:%S.%q} [%8t] %8c %5p -  %m [%M() %b:%L] %n"
                    )));

    m_AppenderContext.getContext()->setLayout(pLayout);
}

FileAppenderImpl::~FileAppenderImpl(void)
{

}

AppenderContext* FileAppenderImpl::getContext(void)
{
    return &m_AppenderContext;
}

} // namespace log
} // namespace mf
} // namespace surveon
