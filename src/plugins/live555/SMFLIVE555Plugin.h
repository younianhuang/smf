
#ifndef SMF_LIVE555_PLUGIN_H
#define SMF_LIVE555_PLUGIN_H

#include "SMFRTSPSourceFactory.h"


namespace surveon
{
namespace mf
{

/**
 */
class LIVE555Plugin : public IPlugin
{
public:
    LIVE555Plugin(MediaManager* pMediaManager);

    virtual ~LIVE555Plugin(void);

    virtual const String& getName(void) const;

    virtual void install(void);

    virtual void uninstall(void);

    virtual void initialize(void);

    virtual void shutdown(void);

private:
    String m_Name;

    MediaManager* m_pMediaManager;

    typedef std::unique_ptr<RTSPSourceFactory> RTSPSourceFactoryPtr;

    RTSPSourceFactoryPtr m_pRTSPSourceFactory;

};


} // namespace mf
} // namespace surveon


#endif // SMF_LIVE555_PLUGIN_H





