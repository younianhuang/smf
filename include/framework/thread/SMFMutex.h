#ifndef SMF_MUTEX_H
#define SMF_MUTEX_H

#include <mutex>
#include <condition_variable>
#include <atomic>

namespace surveon
{
namespace mf
{

class NullMutex
{
public:
    inline void lock() {}
    inline void unlock() {}
    inline bool try_lock() { return true; }
};


typedef std::mutex Mutex;
typedef std::unique_lock<Mutex> MutexLock;
typedef std::recursive_mutex RecursiveMutex;
typedef std::unique_lock<RecursiveMutex> RecursiveMutexLock;
typedef std::condition_variable_any  ConditionVariable;


#define SMF_ATOMIC std::atomic

#define SMF_MUTEX_LOCK(mutex) std::unique_lock<decltype(mutex)> mutex##lock(mutex)

#define SMF_MUTEX_LOCK2(mutex) std::unique_lock<decltype(mutex)> mutexLock(mutex)



} // namespace mf
} // namespace surveon

#endif // SMF_MUTEX_H

