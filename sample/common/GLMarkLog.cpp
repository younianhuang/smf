
#include "GLMarkLog.h"

namespace surveon
{
namespace glmark
{

    Log::Logger* Log::ms_Logger = nullptr;
    bool Log::ms_LogOn = true;


    //=====================================================================

    void Log::setup(const String& loggerName,  const String& filename)
    {
        mf::log::LogManager* pLogManager = mf::log::LogManager::getInstancePtr();
        ms_Logger = pLogManager->getLogger(loggerName);
        mf::log::IAppender* pAppender = pLogManager->createRollingFileAppender(filename);

        ms_Logger->setLogLevel(mf::log::LOG_LEVEL_TRACE);

        ms_Logger->addAppender(pAppender);
    }

} // namespace glmark
} // namespace surveon

