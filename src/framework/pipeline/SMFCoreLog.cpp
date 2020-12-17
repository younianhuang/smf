
#include "SMFCoreLog.h"


namespace surveon
{
namespace mf
{

CoreLog::CoreLog() :
    m_pLogger(nullptr)
{

}

CoreLog::~CoreLog()
{

}

void CoreLog::initialize(const String& filename)
{
    mf::log::LogManager* pLogManager = mf::log::LogManager::getInstancePtr();

    m_pLogger = pLogManager->getLogger("smf");

    mf::log::IAppender* pAppender = pLogManager->createRollingFileAppender(filename);

    m_pLogger->setLogLevel(mf::log::LOG_LEVEL_TRACE);

    m_pLogger->addAppender(pAppender);
}

void CoreLog::shutdown(void)
{

}


} // namespace mf
} // namespace surveon


