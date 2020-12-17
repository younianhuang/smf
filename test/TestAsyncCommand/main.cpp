#include <QCoreApplication>
#include <framework/SMFThread.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace surveon::mf;

class Foo
{
public:
    Foo() :m_Value1(0), m_Value2(0)
    {

    }

    void print()
    {
        cout << "m_Value1 = " << m_Value1 << endl;
        cout << "m_Value2 = " << m_Value2 << endl;
    }

    void setValue(int g_Value1, int g_Value2)
    {
        cout <<__FUNCTION__<<endl;
        m_Value1 = g_Value1;
        m_Value2 = g_Value2;
    }

    int getValue1() const
    {
        cout <<__FUNCTION__<<endl;
        return m_Value1;
    }

    int getValue2() const
    {
        cout <<__FUNCTION__<<endl;
        return m_Value2;
    }

    void testException()
    {
        cout <<__FUNCTION__<<endl;
        SMF_THROW_EXCEPT(InternalErrorException, "Test Exception");
    }

private:
    int m_Value1;
    int m_Value2;
};

Foo g_Foo;
int g_Value1= 3;
int g_Value2 = 5;


AsyncCommandQueue g_CommandQueue;

void setupFoo()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while(g_CommandQueue.hasMoreCommand())
    {
        AsyncCommandPtr pAsyncCommand = g_CommandQueue.getNextCommand();

        pAsyncCommand->invoke();
    }
}

int testFun()
{
    return 0;
}



int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    g_CommandQueue.addCommand(AsyncCommandFactory<>::create(&Foo::setValue, &g_Foo, g_Value1, g_Value2));

    AsyncCommandPtr pAsyncCommand1 = AsyncCommandFactory<int>::create(&Foo::getValue1, &g_Foo) ;
    auto pAsyncResult1 = pAsyncCommand1->getAsyncResult();
    g_CommandQueue.addCommand(pAsyncCommand1);

    AsyncCommandPtr pAsyncCommand2 = AsyncCommandFactory<int>::create(&Foo::getValue2, &g_Foo) ;
    auto pAsyncResult2 = pAsyncCommand2->getAsyncResult();
    g_CommandQueue.addCommand(pAsyncCommand2);

    AsyncCommandPtr pAsyncCommand3 = AsyncCommandFactory<void>::create(&Foo::testException, &g_Foo) ;
    auto pAsyncResult3 = pAsyncCommand3->getAsyncResult();
    g_CommandQueue.addCommand(pAsyncCommand3);

    std::thread  thread1(setupFoo);

    bool throwInternalErrorException = false;
    try
    {
        pAsyncResult3->getReturn<void>();
    }
    catch(InternalErrorException& )
    {
        throwInternalErrorException = true;
    }
     assert(throwInternalErrorException);

    // asyncGetReturn will block until AsyncCommand is done
    int result1 = pAsyncResult1->getReturn<int>();
    assert(result1 == g_Value1);

    int result2 = pAsyncResult2->getReturn<int>();;
    assert(result2 == g_Value2);

    // invalid type test, should throw InvalidTypeException
    bool throwInvalidTypeException = false;
    try
    {
        float f = pAsyncResult1->getReturn<float>();
    }
    catch(InvalidTypeException&)
    {
        throwInvalidTypeException = true;
    }
    assert(throwInvalidTypeException);


    g_Foo.print();

    thread1.join();

    return 0;

    //return a.exec();
}
