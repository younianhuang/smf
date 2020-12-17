#ifndef SMF_AVI_SOURCE_FACTORY_H
#define SMF_AVI_SOURCE_FACTORY_H

#include "SMFffmpegException.h"
#include "SMFAVIFileReader.h"


namespace surveon
{
namespace mf
{

/**
 */
class AVISourceFactory : public IFileSourceFactory
{
public:
    AVISourceFactory(const String& fileType);
    virtual ~AVISourceFactory();

    virtual IMediaSource* create(const String& filename);

    virtual void destroy(IMediaSource* pSource);

    virtual const String& getFileType(void) const;

private:
    void initialize(void);

    void shutdown(void);

    void run(void);

    IMediaSource* createCommand(void);

    void destroyCommand(IMediaSource* pSource);

private:
    String m_FileType;

    DerviedObjectFactory<IMediaSource, AVIFileReader> m_FactoryImpl;

    mf::Thread m_Thread;

    mutable mf::AsyncCommandQueue m_CommandQueue;
    mutable mf::AsyncCommandQueue::SyncControl m_CommandSyncControl;

    bool m_Run;
};

} // namespace mf
} // namespace surveon

#endif // SMF_AVI_SOURCE_FACTORY_H

