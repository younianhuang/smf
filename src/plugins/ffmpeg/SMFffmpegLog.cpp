#include "SMFffmpegLog.h"

namespace surveon
{
namespace mf
{

ffmpegLogManager::ffmpegLogManager() :
    m_pLogger(nullptr)
{

}

ffmpegLogManager::~ffmpegLogManager()
{

}

void ffmpegLogManager::initialize(const String& loggerName,  const String& filename)
{
    mf::log::LogManager* pLogManager = mf::log::LogManager::getInstancePtr();

    m_pLogger = pLogManager->getLogger(loggerName);

    mf::log::IAppender* pAppender = pLogManager->createRollingFileAppender(filename);

    m_pLogger->setLogLevel(mf::log::LOG_LEVEL_TRACE);

    m_pLogger->addAppender(pAppender);
}

void ffmpegLogManager::shutdown(void)
{

}




} // namespace mf
} // namespace surveon



