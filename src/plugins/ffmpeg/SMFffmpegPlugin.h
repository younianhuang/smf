
#ifndef SMF_FFMPEGPlugin_H
#define SMF_FFMPEGPlugin_H

#include "SMFffmpegPrerequisites.h"


namespace surveon
{
namespace mf
{

/**
 */
class ffmpegPlugin : public IPlugin
{
public:
    ffmpegPlugin(MediaManager* pMediaManager);

    virtual ~ffmpegPlugin(void);

    virtual const String& getName(void) const;

    virtual void install(void);

    virtual void uninstall(void);

    virtual void initialize(void);

    virtual void shutdown(void);

private:        
    String m_Name;

    MediaManager* m_pMediaManager;

    typedef std::unique_ptr<IFileSourceFactory> FileSourceFactoryPtr;

    FileSourceFactoryPtr m_pAVISourceFactory;
    FileSourceFactoryPtr m_pMP4SourceFactory;

    std::unique_ptr<IMediaTransformFactory> m_pMediaTransformFactory;
};


} // namespace mf
} // namespace surveon


#endif // SMF_FFMPEGPlugin_H





