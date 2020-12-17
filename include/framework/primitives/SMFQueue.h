#ifndef SMF_QUEUE_H
#define SMF_QUEUE_H

#include "framework/thread/SMFMutex.h"
#include "framework/SMFException.h"

namespace surveon
{
namespace mf
{

template<class T, class MutexType = mf::Mutex>
class Queue
{
public:
    Queue(void) {}

    ~Queue(void)
    {
        flush();
    }

    void add(const T& item)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        m_Items.push_back(item);
    }

    /** Gets and pop item form the queue.
     * @remarks
     * Call this function when this queue is empty will raise exception.
    */
    T getNext(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        if(m_Items.empty())
        {
            SMF_THROW_EXCEPT(InvalidOperationException, "This queue is empty!");
        }

        T item = m_Items.front();
        m_Items.pop_front();
        return item;
    }

    T peek(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        if(m_Items.empty())
        {
            return T();
        }

        return m_Items.front();
    }

    bool hasMore(void) const
    {
        SMF_MUTEX_LOCK(m_Mutex);
        return !m_Items.empty();
    }

    void flush(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        m_Items.clear();
    }

private:
        std::deque<T> m_Items;

        mutable MutexType m_Mutex;
};

} // namespace mf
} // namespace surveon

#endif // SMF_THREAD_SAFE_QUEUE_H

