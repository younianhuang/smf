
#ifndef SMF_PLUGIN_MANAGER_H
#define SMF_PLUGIN_MANAGER_H

#include "framework/pipeline/SMFIPlugin.h"
#include "SMFDynamicLibrary.h"
#include "SMFCorePrerequisites.h"

namespace surveon
{
namespace mf
{

class DynamicLibrary;

/**
 */
class PluginManager
{

public:
    PluginManager(MediaManager* pMediaManager);
    ~PluginManager();

    void initialize(void);
    void shutdown(void);

    void loadPlugin(const String& pluginName);
    void unloadPlugin(const String& pluginName);

    void installPlugin(IPlugin* pPlugin);
    void uninstallPlugin(IPlugin* pPlugin);

private:


    void unloadPlugin(DynamicLibrary* pLibrary);
    void unloadAllPlugins(void);

private:

    typedef std::map<String, DynamicLibrary*> DynamicLibraryMap;
    DynamicLibraryMap m_DynamicLibraryMap;

    typedef std::set<IPlugin*> PluginSet;
    PluginSet m_PluginSet;

    MediaManager* m_pMediaManager;

    bool m_Initialized;

    //Mutex m_Mutex;
};

} // namespace mf
} // namespace surveon



#endif // SMF_PLUGIN_MANAGER_H





