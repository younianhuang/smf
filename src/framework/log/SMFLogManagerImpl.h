
#ifndef SMF_SYSTEM_LOG_MANAGER_IMPL_H
#define SMF_SYSTEM_LOG_MANAGER_IMPL_H

#include "SMFLogImplPrerequisites.h"
#include "framework/SMFThread.h"
#include "SMFLoggerImpl.h"


namespace surveon
{
namespace mf
{
namespace log
{


class LogManagerImpl
{

public:
    LogManagerImpl(void);
    ~LogManagerImpl(void);

    void initialize(void);
    void shutdown(void);

    ILogger* getLogger(const String& name);

    IFileAppender* createFileAppender(const String& filename);
    IRollingFileAppender* createRollingFileAppender(const String& filename);

private:


private:
    typedef std::map<String, std::shared_ptr<LoggerImpl> > LoggerMap;
    LoggerMap m_LoggerMap;

    typedef std::map<String, std::shared_ptr<IFileAppender>> FileAppenderMap;
    FileAppenderMap m_FileAppenderMap;

    typedef std::map<String, std::shared_ptr<IRollingFileAppender>> RollingFileAppenderMap;
    RollingFileAppenderMap m_RollingFileAppenderMap;

    Mutex m_Mutex;
};

} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_SYSTEM_LOG_MANAGER_IMPL_H

