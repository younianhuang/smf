
#ifndef SMF_ASYNC_COMMAND_QUEUE_H
#define SMF_ASYNC_COMMAND_QUEUE_H

#include "SMFAsyncCommand.h"
#include "SMFAsyncResult.h"
#include "SMFMutex.h"

namespace surveon
{
namespace mf
{

class SMF_API AsyncCommandQueue
{
public:

    class SyncControl
    {
    public:

        void waitCommandForMilliSeconds(milli_seconds milliseconds)
        {
            MutexLock lock(m_Mutex);
            m_ConditionVariable.wait_for(lock, std::chrono::milliseconds(milliseconds));
        }

        void waitCommandForMicroSeconds(micro_seconds microSeconds)
        {
            MutexLock lock(m_Mutex);
            m_ConditionVariable.wait_for(lock, std::chrono::microseconds(microSeconds));
        }

        void waitCommand()
        {
            MutexLock lock(m_Mutex);
            m_ConditionVariable.wait(lock);
        }

        void notifyOne(void)
        {
            MutexLock lock(m_Mutex);
            m_ConditionVariable.notify_one();
        }

        void notifyAll(void)
        {
            MutexLock lock(m_Mutex);
            m_ConditionVariable.notify_all();
        }

    private:
        mf::Mutex m_Mutex;
        mf::ConditionVariable m_ConditionVariable;
    };

public:
    AsyncCommandQueue(SyncControl* pSyncControl = nullptr);

    ~AsyncCommandQueue(void);

    void addCommand(AsyncCommandPtr pCommand);

    AsyncCommandPtr getNextCommand(void);

    bool hasMoreCommand(void) const;

    void executeCommands(void);

    template <class Return, class Fn, class... Args>
    AsyncResultPtr addCommand(Fn&& fn, Args&&... arg )
    {
        AsyncCommandPtr pCommand = AsyncCommandFactory<Return>::create(fn, arg...);

        AsyncResultPtr pResult = pCommand->getAsyncResult();

        addCommand(pCommand);

        return pResult;
    }

    template <class Return, class Fn, class... Args>
    inline void asyncCall(AsyncCommand ::CommandType type, Fn&& fn, Args&&... arg )
    {
        AsyncCommandPtr pCommand = AsyncCommandFactory<Return>::create(type, fn, arg...);

        addCommand(pCommand);
    }

    template <class Return, class Fn, class... Args>
    inline Return blockingCall(AsyncCommand ::CommandType type, Fn&& fn, Args&&... arg )
    {
        AsyncCommandPtr pCommand = AsyncCommandFactory<Return>::create(type, fn, arg...);

        AsyncResultPtr pResult = pCommand->getAsyncResult();

        addCommand(pCommand);

        return pResult->getReturn<Return>();
    }

    SyncControl* getSyncControl(void)
    {
        return m_pSyncControl;
    }

private:
        std::deque<AsyncCommandPtr> m_Commands;

        mutable mf::RecursiveMutex m_Mutex;                

        SyncControl* m_pSyncControl;
};


template <class Return, class Fn, class... Args>
void async(AsyncCommandQueue* pCmdQueue, Fn&& fun, Args&&... arg)
{
    assert(pCmdQueue);

    AsyncCommandPtr pCommand = AsyncCommandFactory<Return>::create(fun, arg...);
    pCmdQueue->addCommand(pCommand);
}

template <class Return, class Fn, class... Args>
void async(AsyncCommandQueue* pCmdQueue, AsyncCommand ::CommandType type, Fn&& fun, Args&&... arg )
{
    assert(pCmdQueue);

    AsyncCommandPtr pCommand = AsyncCommandFactory<Return>::create(type, fun, arg...);
    pCmdQueue->addCommand(pCommand);
}

template <class Return, class Fn, class... Args>
Return asyncReturn(AsyncCommandQueue* pCmdQueue, Fn&& fun, Args&&... arg)
{
    assert(pCmdQueue);

    AsyncCommandPtr pCommand = AsyncCommandFactory<Return>::create(fun, arg...);

    AsyncResultPtr pResult = pCommand->getAsyncResult();

    pCmdQueue->addCommand(pCommand);

    return pResult->getReturn<Return>();
}

template <class Return, class Fn, class... Args>
Return asyncReturn(AsyncCommand ::CommandType type, AsyncCommandQueue* pCmdQueue, Fn&& fun, Args&&... arg)
{
    assert(pCmdQueue);

    AsyncCommandPtr pCommand = AsyncCommandFactory<Return>::create(type, fun, arg...);

    AsyncResultPtr pResult = pCommand->getAsyncResult();

    pCmdQueue->addCommand(pCommand);

    return pResult->getReturn<Return>();
}

} // namespace mf
} // namespace surveon

#endif // SMF_ASYNC_COMMAND_QUEUE_H

