#ifndef SMF_LOG_MARCO_H
#define SMF_LOG_MARCO_H

#include "SMFLogPrerequisites.h"
#include "SMFILogger.h"


namespace surveon
{
namespace mf
{
namespace log
{

#define SMF_LOG_MESSAGE(logger, logLevel, msg)  \
    if(logger && logger->isEnabledFor(logLevel))   {  \
        surveon::mf::StringStream ss;   \
        ss << msg;  \
        logger->forcedLog(logLevel, ss.str(), __FILE__, __LINE__, __FUNCTION__);  \
    }

#ifndef SMF_DISABLE_FATAL_LOG
#define SMF_LOG_FATAL(logger, msg)  SMF_LOG_MESSAGE(logger, surveon::mf::log::LOG_LEVEL_FATAL, msg)
#else
#define SMF_LOG_FATAL(logger, msg)
#endif

#ifndef SMF_DISABLE_ERROR_LOG
#define SMF_LOG_ERROR(logger, msg)  SMF_LOG_MESSAGE(logger, surveon::mf::log::LOG_LEVEL_ERROR, msg)
#else
#define SMF_LOG_ERROR(logger, msg)
#endif

#ifndef SMF_DISABLE_WARN_LOG
#define SMF_LOG_WARN(logger, msg)  SMF_LOG_MESSAGE(logger, surveon::mf::log::LOG_LEVEL_WARN, msg)
#else
#define SMF_LOG_WARN(logger, msg)
#endif

#ifndef SMF_DISABLE_INFO_LOG
#define SMF_LOG_INFO(logger, msg)  SMF_LOG_MESSAGE(logger, surveon::mf::log::LOG_LEVEL_INFO, msg)
#else
#define SMF_LOG_INFO(logger, msg)
#endif

#ifndef SMF_DISABLE_DEBUG_LOG
#define SMF_LOG_DEBUG(logger, msg)  SMF_LOG_MESSAGE(logger, surveon::mf::log::LOG_LEVEL_DEBUG, msg)
#else
#define SMF_LOG_DEBUG(logger, msg)
#endif

#ifndef SMF_DISABLE_TRACE_LOG
#define SMF_LOG_TRACE(logger, msg)  SMF_LOG_MESSAGE(logger, surveon::mf::log::LOG_LEVEL_TRACE, msg)
#else
#define SMF_LOG_TRACE(logger, msg)
#endif

#ifdef SMF_PLATFORM_WIN32
#define SMF_ADDR_PREFIX(addr)    "0x" << addr
#else
#define SMF_ADDR_PREFIX(addr)   addr
#endif


} // namespace log
} // namespace mf
} // namespace surveon


#endif // SMF_LOG_MARCO_H

