
#include "SMFLogManagerImpl.h"
#include "SMFLogFileAppenderImpl.h"
#include "SMFRollingFileAppenderImpl.h"

namespace surveon
{
namespace mf
{
namespace log
{

LogManagerImpl::LogManagerImpl(void)
{

}

LogManagerImpl::~LogManagerImpl(void)
{
    shutdown();
}

void LogManagerImpl::initialize(void)
{
    SMF_MUTEX_LOCK(m_Mutex);

    log4cplus::initialize();
}

void LogManagerImpl::shutdown(void)
{
    SMF_MUTEX_LOCK(m_Mutex);

    m_LoggerMap.clear();
    m_FileAppenderMap.clear();
}

ILogger* LogManagerImpl::getLogger(const String& name)
{
    SMF_MUTEX_LOCK(m_Mutex);

    auto& pLogger = m_LoggerMap[name];
    if(!pLogger)
    {
        pLogger.reset(new LoggerImpl(name));
    }

    return pLogger.get();
}

IFileAppender* LogManagerImpl::createFileAppender(const String& filename)
{
    SMF_MUTEX_LOCK(m_Mutex);

    auto& pAppender = m_FileAppenderMap[filename];
    if(!pAppender)
    {
        pAppender.reset(new FileAppenderImpl(filename));
    }

    return pAppender.get();
}

IRollingFileAppender* LogManagerImpl::createRollingFileAppender(const String& filename)
{
    SMF_MUTEX_LOCK(m_Mutex);

    auto& pAppender = m_RollingFileAppenderMap[filename];
    if(!pAppender)
    {
        pAppender.reset(new RollingFileAppenderImpl(filename));
    }

    return pAppender.get();
}

} // namespace log
} // namespace mf
} // namespace surveon
