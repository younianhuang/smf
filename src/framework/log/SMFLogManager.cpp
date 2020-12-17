
#include "framework/log/SMFLogManager.h"
#include "SMFLogManagerImpl.h"

namespace surveon
{
namespace mf
{
namespace log
{

LogManager::LogManager(void) :
    m_pImpl(new LogManagerImpl())
{

}

LogManager::~LogManager(void)
{
    delete m_pImpl;
}

void LogManager::initialize(void)
{
    m_pImpl->initialize();
}

void LogManager::shutdown(void)
{
    m_pImpl->shutdown();
}

ILogger* LogManager::getLogger(const String& name)
{
    return m_pImpl->getLogger(name);
}

IFileAppender* LogManager::createFileAppender(const String& filename)
{
    return m_pImpl->createFileAppender(filename);
}

IRollingFileAppender* LogManager::createRollingFileAppender(const String& filename)
{
    return m_pImpl->createRollingFileAppender(filename);
}

} // namespace log
} // namespace mf
} // namespace surveon


