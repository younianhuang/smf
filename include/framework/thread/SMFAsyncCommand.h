
#ifndef SMF_ASYNC_COMMAND_H
#define SMF_ASYNC_COMMAND_H

#include "framework/SMFPrerequisites.h"
#include "framework/thread/SMFAsyncResult.h"
#include <future>


namespace surveon
{
namespace mf
{

class AsyncCommand
{
public:
    enum CommandType
    {
        CT_GET,
        CT_SET,
        CT_CREATE,
        CT_DESTROY,
        CT_OPEN_FILE,
        CT_CLOSE_FILE,
        CT_RENDER
    };

public:
    AsyncCommand(CommandType type) : m_Type(type)
    {}

    virtual ~AsyncCommand(void) {}

    virtual void invoke(void) = 0;

    virtual AsyncResultPtr getAsyncResult(void) = 0;

    CommandType getType(void) const
    {
        return m_Type;
    }

private:
    CommandType m_Type;
};

typedef std::shared_ptr<AsyncCommand> AsyncCommandPtr;

//===================================================================================================

/**
*/
template<class ReturnType = void>
class GenericAsyncCommand : public AsyncCommand
{
public:
    typedef std::packaged_task<ReturnType()> Actor;

public:
    GenericAsyncCommand(CommandType type) :
        AsyncCommand(type)
    {

    }

    template<class Fn, class... Args>
    explicit GenericAsyncCommand(CommandType type, Fn&& fn, Args&&... arg) :
        AsyncCommand(type)
    {
        m_Actor = Actor(std::bind(fn, arg...));

        m_pAsyncResult.reset(new GenericAsyncResult<ReturnType>(m_Actor.get_future()));                
    }

    virtual void invoke(void)
    {
        m_Actor();
    }

     AsyncResultPtr getAsyncResult(void)
     {
         return m_pAsyncResult;
     }

private:
    Actor m_Actor;
    AsyncResultPtr m_pAsyncResult;
    std::exception_ptr m_pException;
};


//===================================================================================================

template<class ReturnType = void>
class AsyncCommandFactory
{
public:

        template<class Fn, class... Args>
        static AsyncCommandPtr create(AsyncCommand::CommandType type, Fn&& fn, Args&&... arg)
        {
            return AsyncCommandPtr (new GenericAsyncCommand<ReturnType>(type, fn, arg...));
        }

        template<class Fn, class... Args>
        static AsyncCommandPtr create(Fn&& fn, Args&&... arg)
        {
            return AsyncCommandPtr (new GenericAsyncCommand<ReturnType>(AsyncCommand::CT_SET, fn, arg...));
        }
};


} // namespace mf
} // namespace surveon

#endif // SMF_ASYNC_COMMAND_H

