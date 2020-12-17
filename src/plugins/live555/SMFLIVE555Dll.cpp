
#include "SMFLIVE555Plugin.h"

namespace surveon
{
namespace mf
{

static LIVE555Plugin* g_pLIVE555Plugin = nullptr;


extern "C" SMF_LIVE555_API void  dllStartPlugin(MediaManager* pMediaManager)
{
    g_pLIVE555Plugin = new LIVE555Plugin(pMediaManager);

    pMediaManager->installPlugin(g_pLIVE555Plugin);
}

extern "C" SMF_LIVE555_API void  dllStopPlugin(MediaManager* pMediaManager)
{
    pMediaManager->uninstallPlugin(g_pLIVE555Plugin);

    delete g_pLIVE555Plugin;
    g_pLIVE555Plugin = nullptr;
}

} // namespace mf
} // namespace surveon

