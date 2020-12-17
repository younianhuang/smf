#ifndef SMF_LOGGER_IMPL_H
#define SMF_LOGGER_IMPL_H

#include "SMFLogImplPrerequisites.h"
#include "framework/log/SMFILogger.h"

namespace surveon
{
namespace mf
{
namespace log
{

class LoggerImpl : public ILogger
{
public:

    LoggerImpl(const String& name);

    virtual ~LoggerImpl(void);

    virtual const String& getName(void) const;

    virtual void log(LogLevel ll, const String& message,
                const String&file,
                int line,
                const String& function) const;

    virtual void forcedLog(LogLevel ll, const String& message,
                           const String&file,
                           int line,
                           const String& function) const;


     virtual bool isEnabledFor (LogLevel ll) const;

    virtual LogLevel getLogLevel() const;

    virtual void setLogLevel(LogLevel level);

    virtual void addAppender(IAppender* pAppender);

private:
    log4cplus::Logger m_Log4cplusLogger;

};

} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_LOGGER_IMPL_H
