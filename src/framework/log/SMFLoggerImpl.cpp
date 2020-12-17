
#include "SMFLoggerImpl.h"
#include "SMFLogAppenderContext.h"

namespace surveon
{
namespace mf
{
namespace log
{

LoggerImpl::LoggerImpl(const String& name) :
    m_Log4cplusLogger(log4cplus::Logger::getInstance(name.c_str()))
{

}

LoggerImpl::~LoggerImpl()
{

}

const String& LoggerImpl::getName(void) const
{
    return m_Log4cplusLogger.getName();
}

void LoggerImpl::log(LogLevel level, const String& message,
            const String&file,
            int line,
            const String& function) const
{
    m_Log4cplusLogger.log(level, message, file.c_str(), line, function.c_str());
}

void LoggerImpl::forcedLog(LogLevel level, const String& message,
                           const String&file,
                           int line,
                           const String& function) const
{
    m_Log4cplusLogger.forcedLog(level, message, file.c_str(), line, function.c_str());
}


bool LoggerImpl::isEnabledFor (LogLevel level) const
{
    return m_Log4cplusLogger.isEnabledFor(level);
}

LogLevel LoggerImpl::getLogLevel() const
{
    return m_Log4cplusLogger.getLogLevel();
}

void LoggerImpl::setLogLevel(LogLevel level)
{
    m_Log4cplusLogger.setLogLevel(level);
}

void LoggerImpl::addAppender(IAppender* pAppender)
{
    AppenderContext* pContext = pAppender->getContext();

    m_Log4cplusLogger.addAppender(pContext->getContext());
}

} // namespace log
} // namespace mf
} // namespace surveon
