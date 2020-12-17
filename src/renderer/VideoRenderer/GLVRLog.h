#ifndef GLVR_LOG_H
#define GLVR_LOG_H

#include "GLVRPrerequisites.h"
#include <framework/SMFLog.h>

namespace surveon
{

namespace glvr
{

class Log
{
public:
    typedef mf::log::ILogger Logger;

public:
    inline static Logger* getLogger(void)
    {
        return ms_Logger;
    }

    static void setup(const String& loggerName,  const String& filename);

private:
    static Logger* ms_Logger;
};


#define GLVR_LOG_FATAL(msg)     SMF_LOG_FATAL(surveon::glvr::Log::getLogger(), msg)

#define GLVR_LOG_ERROR(msg)     SMF_LOG_ERROR(surveon::glvr::Log::getLogger(), msg)

#define GLVR_LOG_WARN(msg)     SMF_LOG_WARN(surveon::glvr::Log::getLogger(), msg)

#define GLVR_LOG_INFO(msg)     SMF_LOG_INFO(surveon::glvr::Log::getLogger(), msg)

#define GLVR_LOG_DEBUG(msg)     SMF_LOG_DEBUG(surveon::glvr::Log::getLogger(), msg)

#define GLVR_LOG_TRACE(msg)     SMF_LOG_TRACE(surveon::glvr::Log::getLogger(), msg)



} // namespace glvr
} // namespace surveon

#endif // GLVR_LOG_H

