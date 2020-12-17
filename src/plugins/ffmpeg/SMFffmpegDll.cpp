
#include "SMFffmpegPlugin.h"

namespace surveon
{
namespace mf
{

static ffmpegPlugin* g_pffmpegPlugin = nullptr;


extern "C" SMF_FFMPEG_API void  dllStartPlugin(MediaManager* pMediaManager)
{
    g_pffmpegPlugin = new ffmpegPlugin(pMediaManager);

    pMediaManager->installPlugin(g_pffmpegPlugin);
}

extern "C" SMF_FFMPEG_API void  dllStopPlugin(MediaManager* pMediaManager)
{
    pMediaManager->uninstallPlugin(g_pffmpegPlugin);

    delete g_pffmpegPlugin;
    g_pffmpegPlugin = nullptr;
}

} // namespace mf
} // namespace surveon
