#ifndef SMF_ILOGGER_H
#define SMF_ILOGGER_H

#include "SMFLogPrerequisites.h"
#include "SMFLogLevel.h"

namespace surveon
{
namespace mf
{
namespace log
{

class ILogger
{
public:

    virtual ~ILogger(void)   {}

    virtual const String& getName(void) const = 0;

    virtual void log(LogLevel ll, const String& message,
                const String&file,
                int line,
                const String& function) const = 0;

    /**
    * This method creates a new logging event and logs the event
    * without further checks.
    */
   virtual void forcedLog(LogLevel ll, const String& message,
                          const String&file,
                          int line,
                          const String& function) const = 0;

    /**
    * Check whether this logger is enabled for a given LogLevel passed
    * as parameter.
    *
    * @return boolean True if this logger is enabled for <code>ll</code>.
    */
    virtual bool isEnabledFor (LogLevel ll) const  = 0;

    /**
     * Returns the assigned LogLevel, if any, for this Logger.
     *
     * @return LogLevel - the assigned LogLevel, can be <code>NOT_SET_LOG_LEVEL</code>.
     */
    virtual LogLevel getLogLevel() const = 0;

    /**
     * Set the LogLevel of this Logger.
     */
    virtual void setLogLevel(LogLevel ll) = 0;

    virtual void addAppender(IAppender* pAppender) = 0;

private:

};

} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_ILOGGER_H

