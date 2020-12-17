#include "framework/thread/SMFWorkQueue.h"
#include "framework/SMFLog.h"
#include "framework/util/SMFContainerUtil.h"

namespace surveon
{
namespace mf
{

Worker::Worker(AsyncCommandQueue* pCommandQueue) :
    m_Run(false),
    m_pCommandQueue(pCommandQueue),
    m_pSyncControl(pCommandQueue->getSyncControl())
{
    assert(pCommandQueue);
}

Worker::~Worker(void)
{
    stop();
}

void Worker::start(void)
{
    if(!m_Run)
    {
        m_Run = true;
        m_Thread = Thread(&Worker::run, this);
    }
}

void Worker::stop(void)
{
    if(m_Run)
    {
        m_Run = false;
        m_Thread.join();
    }
}

void Worker::run(void)
{
    while(m_Run)
    {
        try
        {
            if(AsyncCommandPtr pCommand = m_pCommandQueue->getNextCommand())
            {
                pCommand->invoke();
            }
            else
            {
                m_pSyncControl->waitCommandForMilliSeconds(10);
            }
        }
        catch(Exception& )
        {

        }
        catch(std::exception& )
        {

        }
        catch(...)
        {

        }
    }
}

//==============================================================

WorkQueue::WorkQueue() :
    m_CommandQueue(&m_SyncControl),
    m_Initialized(false)
{

}

WorkQueue::~WorkQueue()
{
    shutdown();
}

void WorkQueue::initialize(size_t numOfThreads)
{
    SMF_MUTEX_LOCK(m_Mutex);

    assert(!m_Initialized);
    if(!m_Initialized)
    {
        m_Workers.resize(numOfThreads);

        for(size_t i=0; i<m_Workers.size(); i++)
        {
            Worker* pWorker = new Worker(&m_CommandQueue);
            pWorker->start();

            m_Workers[i] = pWorker;
        }

        m_Initialized = true;
    }
}

void WorkQueue::shutdown()
{
    SMF_MUTEX_LOCK(m_Mutex);

    if(m_Initialized)
    {
        ContainerUtil::destroyAllElements(m_Workers);
        m_Initialized = false;
    }
}



} // namespace mf
} // namespace surveon


