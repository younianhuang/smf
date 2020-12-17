#ifndef SMF_CORE_LOG_H
#define SMF_CORE_LOG_H

#include "framework/SMFLog.h"
#include "framework/SMFUtil.h"

namespace surveon
{
namespace mf
{

class CoreLog : public mf::Singleton<CoreLog, Mutex>
{
    friend class Singleton<CoreLog, Mutex>;

public:

    void initialize(const String& filename);
    void shutdown(void);

    inline log::ILogger* getLogger()
    {
        return m_pLogger;
    }

private:
    CoreLog();
    ~CoreLog();

    log::ILogger* m_pLogger;
};


#define SMF_CORE_LOG_FATAL(msg)     SMF_LOG_FATAL(CoreLog::getInstance().getLogger(), msg)

#define SMF_CORE_LOG_ERROR(msg)     SMF_LOG_ERROR(CoreLog::getInstance().getLogger(), msg)

#define SMF_CORE_LOG_WARN(msg)     SMF_LOG_WARN(CoreLog::getInstance().getLogger(), msg)

#define SMF_CORE_LOG_INFO(msg)     SMF_LOG_INFO(CoreLog::getInstance().getLogger(), msg)

#define SMF_CORE_LOG_DEBUG(msg)     SMF_LOG_DEBUG(CoreLog::getInstance().getLogger(), msg)

#define SMF_CORE_LOG_TRACE(msg)     SMF_LOG_TRACE(CoreLog::getInstance().getLogger(), msg)


} // namespace mf
} // namespace surveon

#endif // SMF_CORE_LOG_H


