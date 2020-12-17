
#ifndef SMF_WORKQUEUE_H
#define SMF_WORKQUEUE_H

#include "framework/thread/SMFAsyncCommandQueue.h"
#include "framework/thread/SMFThread.h"

namespace surveon
{
namespace mf
{

class SMF_API Worker
{
public:
    Worker(AsyncCommandQueue* pCommandQueue);
    ~Worker(void);

    void start(void);
    void stop(void);

private:
    void run(void);

private:
    Thread m_Thread;
    bool m_Run;

    AsyncCommandQueue* m_pCommandQueue;
    AsyncCommandQueue::SyncControl* m_pSyncControl;
};


class SMF_API WorkQueue
{
public:


public:
    WorkQueue();
    ~WorkQueue();

    void initialize(size_t numOfThreads = 1);
    void shutdown();

    bool isInitialized(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        return m_Initialized;
    }

    AsyncCommandQueue* getCommandQueue(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        return &m_CommandQueue;
    }

    size_t getNumOfThreads(void) const
    {
        SMF_MUTEX_LOCK(m_Mutex);
        return m_Workers.size();
    }


private:
    AsyncCommandQueue m_CommandQueue;
    AsyncCommandQueue::SyncControl m_SyncControl;

    std::vector<Worker*> m_Workers;

    bool m_Initialized;

    mutable Mutex m_Mutex;

};


} // namespace mf
} // namespace surveon

#endif // SMF_WORKQUEUE_H



