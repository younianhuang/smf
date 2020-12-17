
#include "GLVRLog.h"

namespace surveon
{
namespace glvr
{

    Log::Logger* Log::ms_Logger = nullptr;


    //=====================================================================

    void Log::setup(const String& loggerName,  const String& filename)
    {
        mf::log::LogManager* pLogManager = mf::log::LogManager::getInstancePtr();
        ms_Logger = pLogManager->getLogger(loggerName);
        mf::log::IAppender* pAppender = pLogManager->createRollingFileAppender(filename);

        ms_Logger->setLogLevel(mf::log::LOG_LEVEL_TRACE);

        ms_Logger->addAppender(pAppender);
    }

} // namespace glvr
} // namespace surveon

