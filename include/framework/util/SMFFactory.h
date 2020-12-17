#ifndef SMF_FACTORY_H
#define SMF_FACTORY_H

#include "framework/SMFException.h"
#include "framework/SMFThread.h"
#include "framework/util/SMFContainerUtil.h"
#include "framework/util/SMFSequenceContainerIterator.h"

namespace surveon
{
namespace mf
{

template<class T, class MutexType = NullMutex>
class ObjectList
{
public:
    typedef std::set<T*> ContainterType;
    typedef SequenceContainerIterator<ContainterType> Iterator;

public:

    virtual ~ObjectList(void)
    {
    }

    void destroy(T* pObj)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        if(m_Instances.erase(pObj))
        {
            delete pObj;
        }
        else
        {
            SMF_THROW_EXCEPT(InvalidParametersException, "Specified pointer does not exist!");
        }
    }

    Iterator getIterator(void)
    {
        return Iterator(m_Instances);
    }

    size_t getNumOfItems(void) const
    {
        SMF_MUTEX_LOCK(m_Mutex);
        return m_Instances.size();
    }

    void destroyAll(void)
    {
        SMF_MUTEX_LOCK(m_Mutex);
        ContainerUtil::destroyAllElements(m_Instances);
    }


    void add(T* pObj)
    {
        SMF_MUTEX_LOCK(m_Mutex);

        m_Instances.insert(pObj);
    }

    MutexType& getMutex(void) const
    {
        return m_Mutex;
    }

protected:
    std::set<T*> m_Instances;
    mutable MutexType m_Mutex;
};

template<class T, class MutexType = NullMutex>
class SimpleObjectFactory : public ObjectList<T, MutexType>
{
public:
    typedef ObjectList<T, MutexType> ObjectListType;

public:
    virtual ~SimpleObjectFactory(void)
    {
        ObjectListType::destroyAll();
    }

    template<class... Args>
    T* create(Args&&... arg)
    {
        SMF_MUTEX_LOCK2(ObjectListType::m_Mutex);

        T* pObj = new T(arg...);
        ObjectListType::m_Instances.insert(pObj);
        return pObj;
    }
};

template<class T, class MutexType = NullMutex>
class HierarchyObjectFactory : public ObjectList<T, MutexType>
{
public:
    typedef ObjectList<T, MutexType> ObjectListType;

public:
    ~HierarchyObjectFactory(void)
    {
        ObjectListType::destroyAll();
    };

    template<class Dervied, class... Args>
    T* create(Args&&... arg)
    {
        SMF_MUTEX_LOCK2(ObjectListType::m_Mutex);

        T* pObj = new Dervied(arg...);
        ObjectListType::m_Instances.insert(pObj);
        return pObj;
    }
};


template<class BaseType, class DerviedType, class MutexType = NullMutex>
class DerviedObjectFactory : public ObjectList<DerviedType, MutexType>
{
public:
    typedef ObjectList<DerviedType, MutexType> ObjectListType;


public:

    ~DerviedObjectFactory(void)
    {
        ObjectListType::destroyAll();
    };

    template<class... Args>
    BaseType* create(Args&&... arg)
    {        
        SMF_MUTEX_LOCK2(ObjectListType::m_Mutex);

        DerviedType* pObj = new DerviedType(arg...);
        ObjectListType::m_Instances.insert(pObj);
        return pObj;
    }

    void destroy(BaseType* pObj)
    {
        ObjectListType::destroy(static_cast<DerviedType*>(pObj));
    }
};

} // namespace mf
} // namespace surveon


#endif // SMF_FACTORY_H
