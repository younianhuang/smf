
#ifndef SMF_PLUGIN_H
#define SMF_PLUGIN_H

#include "framework/SMFPrerequisites.h"


namespace surveon
{
namespace mf
{

class MediaManager;

/**
 */
class IPlugin
{
public:
    virtual ~IPlugin(void) {}

    virtual const String& getName(void) const  = 0;

    virtual void install(void) = 0;

    virtual void uninstall(void) = 0;

    virtual void initialize(void) = 0;

    virtual void shutdown(void) = 0;
};


/// dll entry function declare
//extern "C" void  dllStartPlugin(void);
//extern "C" void  dllStopPlugin(void);

#define SMF_STR_DLL_START_FN "dllStartPlugin"
#define SMF_STR_DLL_STOP_FN "dllStopPlugin"

typedef void (*FP_DLL_START_PLUGIN)(MediaManager* pMediaManager);
typedef void (*FP_DLL_STOP_PLUGIN)(MediaManager* pMediaManager);


} // namespace mf
} // namespace surveon



#endif // SMF_PLUGIN_H



