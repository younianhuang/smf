#ifndef GLMARK_LOG_H
#define GLMARK_LOG_H

#include <framework/SMFLog.h>

namespace surveon
{

namespace glmark
{
    using surveon::mf::String;

class Log
{
public:
    typedef mf::log::ILogger Logger;

public:
    inline static Logger* getLogger(void)
    {
        return ms_Logger;
    }

    inline static bool getLogOn(void)
    {
        return ms_LogOn;
    }

    static void setLogOn(bool logOn)
    {
        ms_LogOn = logOn;
    }

    static void setup(const String& loggerName,  const String& filename);

private:
    static Logger* ms_Logger;
    static bool ms_LogOn;
};


#define GLMARK_LOG_FATAL(msg) \
    if (surveon::glmark::Log::getLogOn()) \
        SMF_LOG_FATAL(surveon::glmark::Log::getLogger(), msg)

#define GLMARK_LOG_ERROR(msg) \
    if (surveon::glmark::Log::getLogOn()) \
        SMF_LOG_ERROR(surveon::glmark::Log::getLogger(), msg)

#define GLMARK_LOG_WARN(msg) \
    if (surveon::glmark::Log::getLogOn()) \
        SMF_LOG_WARN(surveon::glmark::Log::getLogger(), msg)

#define GLMARK_LOG_INFO(msg) \
    if (surveon::glmark::Log::getLogOn()) \
        SMF_LOG_INFO(surveon::glmark::Log::getLogger(), msg)

#define GLMARK_LOG_DEBUG(msg) \
    if (surveon::glmark::Log::getLogOn()) \
        SMF_LOG_DEBUG(surveon::glmark::Log::getLogger(), msg)

#define GLMARK_LOG_TRACE(msg) \
    if (surveon::glmark::Log::getLogOn()) \
        SMF_LOG_TRACE(surveon::glmark::Log::getLogger(), msg)



} // namespace glmark
} // namespace surveon

#endif // GLMARK_LOG_H

