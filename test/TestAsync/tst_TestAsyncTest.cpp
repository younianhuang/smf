#include <QString>
#include <QtTest>

#include <framework/SMFThread.h>
#include <iostream>
#include <thread>
#include <string>

using namespace std;
using surveon::mf::AsyncCommandQueue;
using surveon::mf::AsyncCommand;
using surveon::mf::AsyncCommandPtr;
using surveon::mf::AsyncCommandFactory;

const int INT_VALUE = 54321;
const float FLOAT_VALUE = 123.456f;
const string STRING_VALUE = "This is a async command test";
int g_NonMemberValue = 0;

class TestAsyncTest : public QObject
{
    Q_OBJECT

public:
    TestAsyncTest();

    void executeCommands();

private Q_SLOTS:
    void initTestCase();
    void testBindNonMemberFunction();
    void testBindMemberFunctionInt();
    void testBindMemberFunctionFloat();
    void testBindMemberFunctionString();
    void cleanupTestCase();


    void setIntValue(int value)
    {
        m_IntValue = value;
    }

    int getIntValue()
    {
        return m_IntValue;
    }

    void setFloatValue(float value)
    {
        m_FloatValue = value;
    }

    float getFloatValue()
    {
        return m_FloatValue;
    }

    void setStringValue(const std::string& value)
    {
        m_StringValue = value;
    }

    string getStringValue()
    {
        return m_StringValue;
    }

private:
    AsyncCommandQueue m_AsyncCommandQueue;

    int m_NonMemberValue;
    int m_IntValue;
    float m_FloatValue;
    string m_StringValue;

    std::thread m_Thread;
    bool m_Run;
};


//============================================================================
void nonMemberFun()
{
    g_NonMemberValue = INT_VALUE;
}


TestAsyncTest::TestAsyncTest() :
    m_NonMemberValue(0),
    m_IntValue(0),
    m_FloatValue(0.0),
    m_Run(true)
{

}

void TestAsyncTest::executeCommands()
{
    while(m_Run)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if(m_AsyncCommandQueue.hasMoreCommand())
        {
            AsyncCommandPtr pAsyncCommand = m_AsyncCommandQueue.getNextCommand();

            pAsyncCommand->invoke();
        }
    }
}

//=============================================================================

 void TestAsyncTest::initTestCase()
 {

     m_Thread = std::thread(&TestAsyncTest::executeCommands, this);
 }

 void TestAsyncTest::cleanupTestCase()
 {
     m_Run = false;
     m_Thread.join();
 }


void TestAsyncTest::testBindNonMemberFunction()
{
    AsyncCommandPtr pCommand = AsyncCommandFactory<>::create(AsyncCommand::CT_SET, nonMemberFun);
    auto pResult = pCommand->getAsyncResult();
    m_AsyncCommandQueue.addCommand(pCommand);

    pResult->wait();

    QVERIFY2(g_NonMemberValue == INT_VALUE, "Failure");
}

void TestAsyncTest::testBindMemberFunctionInt()
{
    AsyncCommandPtr pCommand = AsyncCommandFactory<>::create(AsyncCommand::CT_SET,
                                                             &TestAsyncTest::setIntValue, this, INT_VALUE);
    m_AsyncCommandQueue.addCommand(pCommand);

    pCommand = AsyncCommandFactory<int>::create(AsyncCommand::CT_GET, &TestAsyncTest::getIntValue, this);
    m_AsyncCommandQueue.addCommand(pCommand);

    auto pResult = pCommand->getAsyncResult();

    QVERIFY2(pResult->getReturn<int>() == INT_VALUE , "Failure");
}

void TestAsyncTest::testBindMemberFunctionFloat()
{
    AsyncCommandPtr pCommand = AsyncCommandFactory<>::create(AsyncCommand::CT_SET,
                                                             &TestAsyncTest::setFloatValue, this, FLOAT_VALUE);
    m_AsyncCommandQueue.addCommand(pCommand);

    pCommand = AsyncCommandFactory<float>::create(AsyncCommand::CT_GET,
                                                  &TestAsyncTest::getFloatValue, this);
    m_AsyncCommandQueue.addCommand(pCommand);

    auto pResult = pCommand->getAsyncResult();

    QVERIFY2(pResult->getReturn<float>() == FLOAT_VALUE , "Failure");
}

void TestAsyncTest::testBindMemberFunctionString()
{
    AsyncCommandPtr pCommand = AsyncCommandFactory<>::create(AsyncCommand::CT_SET,
                                                             &TestAsyncTest::setStringValue, this, STRING_VALUE);
    m_AsyncCommandQueue.addCommand(pCommand);

    pCommand = AsyncCommandFactory<string>::create(AsyncCommand::CT_GET,
                                                   &TestAsyncTest::getStringValue, this);
    m_AsyncCommandQueue.addCommand(pCommand);

    auto pResult = pCommand->getAsyncResult();

     QVERIFY2(pResult->getReturn<string>() == STRING_VALUE , "Failure");
}

QTEST_APPLESS_MAIN(TestAsyncTest)

#include "tst_TestAsyncTest.moc"
