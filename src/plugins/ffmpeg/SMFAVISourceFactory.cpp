#include "SMFAVISourceFactory.h"

namespace surveon
{
namespace mf
{

/**
 */
AVISourceFactory::AVISourceFactory(const String& fileType) :
    m_FileType(fileType),
    m_Run(false),
    m_CommandQueue(&m_CommandSyncControl)
{

}

AVISourceFactory::~AVISourceFactory()
{
    shutdown();
}


IMediaSource* AVISourceFactory::create(const String& filename)
{
    if(!m_Run)
    {
        initialize();
    }

    IMediaSource* pSource = m_CommandQueue.blockingCall<IMediaSource*>(
                AsyncCommand::CT_CREATE,
                &AVISourceFactory::createCommand, this);

    pSource->initialize(filename);

    return pSource;
}

 void AVISourceFactory::destroy(IMediaSource* pSource)
 {
    assert(pSource);

    m_CommandQueue.blockingCall<void>(
                    AsyncCommand::CT_DESTROY,
                    &AVISourceFactory::destroyCommand, this, pSource);
 }

const String& AVISourceFactory::getFileType(void) const
{
    return m_FileType;
}

//=================================================================================

void AVISourceFactory::initialize(void)
{
    SMF_FFMPEG_LOG_INFO( "Initialize AVISourceFactory" << SMF_ADDR_PREFIX(this));

    m_Run = true;
    m_Thread = Thread(&AVISourceFactory::run, this);
}

IMediaSource* AVISourceFactory::createCommand(void)
{
    return m_FactoryImpl.create(&m_CommandQueue);
}

 void AVISourceFactory::destroyCommand(IMediaSource* pSource)
 {
     m_FactoryImpl.destroy(pSource);
 }

void AVISourceFactory::shutdown(void)
{
    if(m_Run)
    {
        SMF_FFMPEG_LOG_INFO( "Shutdown AVISourceFactory " << SMF_ADDR_PREFIX(this));

        m_Run = false;
        m_Thread.join();
    }
}

void AVISourceFactory::run(void)
{
    while(m_Run)
    {
        m_CommandQueue.executeCommands();

        auto it = m_FactoryImpl.getIterator();

        bool buffFulled = true;

        while (it.hasMore())
        {
            AVIFileReader* pAVIFileReader = it.getNext();

            if( pAVIFileReader->getStreamStatus() == AVIFileReader::SS_OK  &&
                !pAVIFileReader->isBufferFulled())
            {
                pAVIFileReader->readPackets();

                if(!pAVIFileReader->isBufferFulled())
                {
                    buffFulled = false;
                }
            }
        }

        if(buffFulled && !m_CommandQueue.hasMoreCommand())
        {
            m_CommandSyncControl.waitCommandForMilliSeconds(10);
        }
    }

    m_FactoryImpl.destroyAll();
}

} // namespace mf
} // namespace surveon



