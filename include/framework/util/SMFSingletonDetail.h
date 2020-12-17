
#ifndef SMF_SINGLETON_DETAIL
#define SMF_SINGLETON_DETAIL

namespace surveon
{
namespace mf
{


//==============================================================
// static member data definition
//==============================================================
template <class T, class MutexType>
T* Singleton<T, MutexType>::ms_Instance = nullptr;

template <class T, class MutexType>
MutexType Singleton<T, MutexType>::ms_Mutex;

//==============================================================
// member function definition
//==============================================================
template <class T, class MutexType>
T& Singleton<T, MutexType>::getInstance(void)
{
    return *getInstancePtr();
}

template <class T, class MutexType>
T* Singleton<T, MutexType>::getInstancePtr(void)
{
    SMF_MUTEX_LOCK(ms_Mutex);
    if (!ms_Instance)
    {
        ms_Instance = new T();
    }

    return ms_Instance;
}

template <class T, class MutexType>
void Singleton<T, MutexType>::destroyInstance(void)
{
    SMF_MUTEX_LOCK(ms_Mutex);
    delete ms_Instance;
    ms_Instance = nullptr;
}

} // namespace mf
} // namespace surveon

#endif // SMF_SINGLETON_DETAIL

