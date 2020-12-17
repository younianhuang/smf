
#ifndef SMF_FFMPEG_LOG_H
#define SMF_FFMPEG_LOG_H

#include "SMFffmpegPrerequisites.h"

namespace surveon
{
namespace mf
{

/**
 */
class ffmpegLogManager : public mf::Singleton<ffmpegLogManager, Mutex>
{
    friend class Singleton<ffmpegLogManager, Mutex>;

public:

    void initialize(const String& loggerName,  const String& filename);
    void shutdown(void);

    inline log::ILogger* getLogger()
    {
        return m_pLogger;
    }

private:
    ffmpegLogManager();
    ~ffmpegLogManager();

    log::ILogger* m_pLogger;
};


#define SMF_FFMPEG_LOG_FATAL(msg)     SMF_LOG_FATAL(ffmpegLogManager::getInstance().getLogger(), msg)

#define SMF_FFMPEG_LOG_ERROR(msg)     SMF_LOG_ERROR(ffmpegLogManager::getInstance().getLogger(), msg)

#define SMF_FFMPEG_LOG_WARN(msg)     SMF_LOG_WARN(ffmpegLogManager::getInstance().getLogger(), msg)

#define SMF_FFMPEG_LOG_INFO(msg)     SMF_LOG_INFO(ffmpegLogManager::getInstance().getLogger(), msg)

#define SMF_FFMPEG_LOG_DEBUG(msg)     SMF_LOG_DEBUG(ffmpegLogManager::getInstance().getLogger(), msg)

#define SMF_FFMPEG_LOG_TRACE(msg)     SMF_LOG_TRACE(ffmpegLogManager::getInstance().getLogger(), msg)

} // namespace mf
} // namespace surveon


#endif // SMF_FFMPEG_LOG_H

