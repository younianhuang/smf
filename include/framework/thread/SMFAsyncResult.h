
#ifndef SMF_ASYNC_RESULT_H
#define SMF_ASYNC_RESULT_H

#include "framework/SMFPrerequisites.h"
#include "framework/SMFException.h"
#include <future>
#include <typeinfo>

namespace surveon
{
namespace mf
{
template<class T>
class GenericAsyncResult;

class AsyncResult
{
public:
    virtual ~AsyncResult(void)   {}

    virtual void wait(void) = 0;

    virtual const std::type_info& getReturnType(void) const = 0;

    template<class T>
    T getReturn()
    {
        if(this->getReturnType() == typeid(T))
        {
             return static_cast<GenericAsyncResult<T>*>(this)->getReturnValue();
        }
        else
        {
            SMF_THROW_EXCEPT(InvalidTypeException, "Invalid Return Type");
        }
    }

    template<class T>
    T getReturn() const
    {
        if(this->getReturnType() == typeid(T))
        {
             return static_cast<GenericAsyncResult<T>*>(this)->getReturnValue();
        }
        else
        {
            SMF_THROW_EXCEPT(InvalidTypeException, "Invalid Return Type");
        }
    }

};

typedef std::shared_ptr<AsyncResult> AsyncResultPtr;

/**
*/
template<class T>
class GenericAsyncResult : public AsyncResult
{
public:
    typedef std::future<T> Future;

public:
    GenericAsyncResult(void) : AsyncResult()
    {

    }

    GenericAsyncResult(Future&& future)
    {
        m_Future = std::move(future);
    }

    T getReturnValue(void)
    {
        return m_Future.get();
    }

    T getReturnValue(void) const
    {
        return m_Future.get();
    }

    virtual void wait(void)
    {
        m_Future.wait();
    }

    virtual const std::type_info& getReturnType(void) const
    {
        return typeid(T);
    }

private:
    Future m_Future;
};


/*

template<class T>
T asyncGetReturn(AsyncResult* pAsyncResult)
{
    if(pAsyncResult->getReturnType() == typeid(T))
    {
         return static_cast<GenericAsyncResult<T>*>(pAsyncResult)->getReturnValue();
    }
    else
    {
        SMF_THROW_EXCEPT(InvalidTypeException, "Invalid Return Type");
    }
}

template<class T>
T asyncGetReturn(AsyncResultPtr& pAsyncResult)
{
    return asyncGetReturn<T>(pAsyncResult.get());
};
*/

} // namespace mf
} // namespace surveon

#endif // SMF_ASYNC_RESULT_H

