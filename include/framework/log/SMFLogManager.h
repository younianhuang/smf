
#ifndef SMF_SYSTEM_LOG_MANAGER_H
#define SMF_SYSTEM_LOG_MANAGER_H

#include "SMFLogPrerequisites.h"
#include "framework/util/SMFSingleton.h"


namespace surveon
{
namespace mf
{
namespace log
{

class LogManagerImpl;

class SMF_API LogManager : public Singleton<LogManager, Mutex>
{
    friend class Singleton<LogManager, Mutex>;

public:
    void initialize(void);
    void shutdown(void);

    ILogger* getLogger(const String& name);
    IFileAppender* createFileAppender(const String& filename);
    IRollingFileAppender* createRollingFileAppender(const String& filename);

private:
    LogManager(void);
    ~LogManager(void);

    LogManagerImpl* m_pImpl;
};

} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_SYSTEM_LOG_MANAGER_H

