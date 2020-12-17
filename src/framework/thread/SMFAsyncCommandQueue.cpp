

#include "framework/thread/SMFAsyncCommandQueue.h"


namespace surveon
{
namespace mf
{

    AsyncCommandQueue::AsyncCommandQueue(SyncControl* pSyncControl) :
        m_pSyncControl(pSyncControl)
    {

    }

    AsyncCommandQueue::~AsyncCommandQueue(void)
    {
        //if(m_pSyncControl)
        //    m_pSyncControl->notifyAll();
    }

    void AsyncCommandQueue::addCommand(AsyncCommandPtr pCommand)
    {
        {
            SMF_MUTEX_LOCK(m_Mutex);
            m_Commands.push_back(pCommand);
        }

        if(m_pSyncControl)
            m_pSyncControl->notifyOne();
    }

    AsyncCommandPtr AsyncCommandQueue::getNextCommand(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);

        AsyncCommandPtr pCmd;

        if(!m_Commands.empty())
        {
            pCmd = m_Commands.front();

            m_Commands.pop_front();
        }

        return pCmd;
    }

    bool AsyncCommandQueue::hasMoreCommand(void) const
    {
        SMF_MUTEX_LOCK(m_Mutex);
        return !m_Commands.empty();
    }

    void AsyncCommandQueue::executeCommands(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        while(!m_Commands.empty())
        {
            AsyncCommandPtr pAsyncCommand = getNextCommand();

            pAsyncCommand->invoke();
        }
    }


} // namespace mf
} // namespace surveon



