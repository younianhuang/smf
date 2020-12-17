#include "SMFPluginManager.h"


namespace surveon
{
namespace mf
{

PluginManager::PluginManager(MediaManager* pMediaManager) :
    m_pMediaManager(pMediaManager),
    m_Initialized(false)
{

}

PluginManager::~PluginManager()
{
    shutdown();
}

void PluginManager::initialize(void)
{
    if(m_Initialized)
        return;

    for(IPlugin* pPlugin : m_PluginSet)
    {
        pPlugin->initialize();
    }

    m_Initialized = true;
}

void PluginManager::shutdown(void)
{
    if(m_Initialized)
    {
        unloadAllPlugins();

        m_Initialized = false;
    }
}

void PluginManager::loadPlugin(const String& pluginName)
{
    if(m_DynamicLibraryMap.count(pluginName) != 0)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException,
                              "Plugin "  << pluginName  << " is already loaded");
    }

    DynamicLibrary* pLibrary = new DynamicLibrary(pluginName);
    m_DynamicLibraryMap.insert(DynamicLibraryMap::value_type(pluginName, pLibrary));

    pLibrary->load();

    FP_DLL_START_PLUGIN pFunc = (FP_DLL_START_PLUGIN)pLibrary->getSymbol(SMF_STR_DLL_START_FN);
    if(!pFunc)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException,
                              "Cannot find symbol " << SMF_STR_DLL_START_FN << " in library "  << pluginName);
    }
    pFunc(m_pMediaManager);

}

void PluginManager::unloadPlugin(const String& pluginName)
{
    DynamicLibraryMap::iterator it =m_DynamicLibraryMap.find(pluginName);
    if(it == m_DynamicLibraryMap.end())
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException,
                              "Plugin "  << pluginName  << " is not loaded");
    }

    unloadPlugin(it->second);

    m_DynamicLibraryMap.erase(it);
}

void PluginManager::unloadPlugin(DynamicLibrary* pLibrary)
{
    FP_DLL_START_PLUGIN pFunc = (FP_DLL_START_PLUGIN)pLibrary->getSymbol(SMF_STR_DLL_STOP_FN);
    if(!pFunc)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException,
                              "Cannot find symbol " << SMF_STR_DLL_STOP_FN << "  in library "  << pLibrary->getName());
    }
    pFunc(m_pMediaManager);

    delete pLibrary;
}

void PluginManager::unloadAllPlugins(void)
{
    for(auto& it : m_DynamicLibraryMap)
    {
        unloadPlugin(it.second);
    }

    m_DynamicLibraryMap.clear();
}

void PluginManager::installPlugin(IPlugin* pPlugin)
{
    assert(pPlugin);

    SMF_CORE_LOG_INFO("Install plugin " << pPlugin->getName());

    pPlugin->install();

    if(m_Initialized)
        pPlugin->initialize();

    m_PluginSet.insert(pPlugin);
}

void PluginManager::uninstallPlugin(IPlugin* pPlugin)
{
    assert(pPlugin);

    SMF_CORE_LOG_INFO("Uninstall plugin " << pPlugin->getName());

    pPlugin->shutdown();

    pPlugin->uninstall();

    m_PluginSet.erase(pPlugin);
}


} // namespace mf
} // namespace surveon





