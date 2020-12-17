#include <QString>
#include <QtTest>
#include <framework/SMFPrimitives.h>

using surveon::mf::Any;
using surveon::mf::Attributes;

class TestAttributeTest : public QObject
{
    Q_OBJECT

public:
    TestAttributeTest();

private Q_SLOTS:
    void testAny();
    void testAttributes();
};

TestAttributeTest::TestAttributeTest()
{
}

void TestAttributeTest::testAny()
{    
    Any any;

    int intValue = 123;
    float fValue = 134.53453f;
    std::string strValue("This is a string");

    QVERIFY2(any.isEmpty(), "Failure");
    QVERIFY2(any.getType()== typeid(void), "Failure");

    any = intValue;
    QVERIFY2(any.getType() == typeid(intValue), "Failure");
    QVERIFY2(any.getContentAs<int>() == intValue, "Failure");

    any = fValue;
    QVERIFY2(any.getType() == typeid(fValue), "Failure");
    QVERIFY2(any.getContentAs<float>() == fValue, "Failure");

    any = strValue;
    QVERIFY2(any.getType() == typeid(strValue), "Failure");
    QVERIFY2(any.getContentAs<std::string>() == strValue, "Failure");

    QVERIFY_EXCEPTION_THROWN(any.getContentAs<int>(), surveon::mf::InvalidTypeException);

}

void TestAttributeTest::testAttributes()
{
    Attributes attributes;
    int attri1 = 93489;
    double attri2 = 3489.345;
    std::string attri3 = "attri3";

    attributes.setAttribuate("attri1", attri1);
    QVERIFY2(attributes.getAttribuate<int>("attri1") == attri1, "Failure");
    QVERIFY2(attributes.hasAttribute("attri1"), "Failure");

    attributes.setAttribuate("attri2", attri2);
    QVERIFY2(attributes.getAttribuate<double>("attri2") == attri2, "Failure");

    attributes.setAttribuate(attri3, attri3);
    QVERIFY2(attributes.getAttribuate<std::string>(attri3) == attri3, "Failure");

    QVERIFY2(!attributes.hasAttribute("non"), "Failure");
    QVERIFY_EXCEPTION_THROWN(attributes.getAttribuate<std::string>("non"),
                             surveon::mf::InvalidParametersException);
    QVERIFY_EXCEPTION_THROWN(attributes.setAttribuate<std::string>("attri1", attri3),
                             surveon::mf::InvalidTypeException);

}

QTEST_APPLESS_MAIN(TestAttributeTest)

#include "tst_TestAttributeTest.moc"
