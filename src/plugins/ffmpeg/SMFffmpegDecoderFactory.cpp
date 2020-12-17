#include "SMFffmpegDecoderFactory.h"

namespace surveon
{
namespace mf
{

ffmpegDecoderFactory::ffmpegDecoderFactory() :
    m_pCommandQueue(nullptr)
{
    m_Attributes.setAttribuate(ATTRI_NUMBER_OF_THREAD, 4);

    m_Attributes.setAttributeChangedCallback(&ffmpegDecoderFactory::onAttributeChanged, this);
}

ffmpegDecoderFactory::~ffmpegDecoderFactory()
{
    MutexLock(m_Mutex);

    m_FactoryImpl.destroyAll();

    m_WorkQueue.shutdown();
}

IMediaTransform* ffmpegDecoderFactory::create(void)
{
    MutexLock(m_Mutex);

    if(!m_WorkQueue.isInitialized())
    {
        int num = m_Attributes.getAttribuate<int>(ATTRI_NUMBER_OF_THREAD);

        m_WorkQueue.initialize(num);

        m_pCommandQueue = m_WorkQueue.getCommandQueue();
    }

    return m_FactoryImpl.create(m_pCommandQueue);
}

void ffmpegDecoderFactory::destroy(IMediaTransform* pTransform)
{
    MutexLock(m_Mutex);

    m_FactoryImpl.destroy(pTransform);
}

const String& ffmpegDecoderFactory::getTypeId(void) const
{
    return ffmpegVideoDecoder::typeId();
}

Attributes* ffmpegDecoderFactory::getAttributes(void)
{
    MutexLock(m_Mutex);
    return &m_Attributes;
}

//==========================================================================

void ffmpegDecoderFactory::onAttributeChanged(const String& name)
{
    AsyncCommandQueue* pCommandQueue =  MediaManager::getInstancePtr()->getCommandQueue();

    async<void>(pCommandQueue, &ffmpegDecoderFactory::attributeChangedCommand, this, name);
}

void ffmpegDecoderFactory::attributeChangedCommand(const String& name)
{
    if(name == ATTRI_NUMBER_OF_THREAD)
    {
        if(m_WorkQueue.isInitialized())
        {
            int num = m_Attributes.getAttribuate<int>(ATTRI_NUMBER_OF_THREAD);
            if(num != m_WorkQueue.getNumOfThreads())
            {
                m_WorkQueue.shutdown();
                m_WorkQueue.initialize(num);
            }
        }
    }
}


} // namespace mf
} // namespace surveon



