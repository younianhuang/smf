#include <QString>
#include <QtTest>
#include <framework/SMFPrimitives.h>
#include<framework/SMFException.h>


using surveon::mf::uint8;
using surveon::mf::IMediaBufferFactory;
using surveon::mf::IMediaBuffer;
using surveon::mf::IVideoBuffer;
using surveon::mf::WrapperVideoBuffer;
using surveon::mf::PlaneInfo;

class TestVideoBufferTest : public QObject
{
    Q_OBJECT

public:
    TestVideoBufferTest();

private Q_SLOTS:
    void testMediaBuffer();
    void testVideoBufferRGB32();
    void testVideoBufferYV12();
    void testWrapperVideoBuffer();
};

TestVideoBufferTest::TestVideoBufferTest()
{
}

void TestVideoBufferTest::testMediaBuffer()
{
    uint8 data[] = "This is a buffer for testing media buffer allocation";
    size_t dataSize = sizeof(data);

    IMediaBufferFactory* pMediaBufferFactory =  surveon::mf::getSystemMemoryMediaBufferFactory();
    IMediaBuffer* pMediaBuffer =  pMediaBufferFactory->createMediaBuffer(dataSize);

    QVERIFY2(pMediaBuffer->getMaxLength() == dataSize, "Failure");

    uint8* buffer = pMediaBuffer->lock();

    memcpy(buffer, data, dataSize);
    pMediaBuffer->setCurrentLength(dataSize);
    QVERIFY2(pMediaBuffer->getCurrentLength() == dataSize, "Failure");

    QVERIFY2(buffer[0] == data[0], "Failure");
    QVERIFY2(buffer[5] == data[5], "Failure");
    QVERIFY2(buffer[dataSize-1] == data[dataSize-1], "Failure");

    pMediaBuffer->unlock();

    pMediaBufferFactory->destroyMediaBuffer(pMediaBuffer);

    QVERIFY_EXCEPTION_THROWN(pMediaBufferFactory->destroyMediaBuffer(pMediaBuffer), surveon::mf::InvalidParametersException);
}

void TestVideoBufferTest::testVideoBufferRGB32()
{
    IMediaBufferFactory* pMediaBufferFactory =  surveon::mf::getSystemMemoryMediaBufferFactory();
    IVideoBuffer* pVideoBuffer =  pMediaBufferFactory->createVideoBuffer(1920, 1080, surveon::mf::SMF_VIDEO_FORMAT_RGBA);

    QVERIFY2(pVideoBuffer->getWidth() == 1920, "Failure");
    QVERIFY2(pVideoBuffer->getHeight()  == 1080, "Failure");

    const PlaneInfo& planeInfo = pVideoBuffer->lockPlane();
     QVERIFY2(planeInfo.getNumberOfPlanes() == 1, "Failure");

    const PlaneInfo::PlaneData& plantData = planeInfo.getPlaneData(0);

    QVERIFY2(plantData.pixels, "Failure");
    QVERIFY2(plantData.width == pVideoBuffer->getWidth(), "Failure");
    QVERIFY2(plantData.height == pVideoBuffer->getHeight(), "Failure");
    QVERIFY2(plantData.pitch == pVideoBuffer->getWidth() * 4 , "Failure");

    pVideoBuffer->unlockPlane();

    pMediaBufferFactory->destroyVideoBuffer(pVideoBuffer);
}


void TestVideoBufferTest::testVideoBufferYV12()
{
    IMediaBufferFactory* pMediaBufferFactory =  surveon::mf::getSystemMemoryMediaBufferFactory();
    IVideoBuffer* pVideoBuffer =  pMediaBufferFactory->createVideoBuffer(1920, 1080, surveon::mf::SMF_VIDEO_FORMAT_YV12);
    QVERIFY2(pVideoBuffer->getWidth() == 1920, "Failure");
    QVERIFY2(pVideoBuffer->getHeight()  == 1080, "Failure");

    const PlaneInfo& planeInfo = pVideoBuffer->lockPlane();
    QVERIFY2(planeInfo.getNumberOfPlanes() == 3, "Failure");

    const PlaneInfo::PlaneData& planeDataY = planeInfo.getPlaneData(0);
    QVERIFY2(planeDataY.pixels, "Failure");
    QVERIFY2(planeDataY.width == pVideoBuffer->getWidth(), "Failure");
    QVERIFY2(planeDataY.height == pVideoBuffer->getHeight(), "Failure");
    QVERIFY2(planeDataY.pitch == pVideoBuffer->getWidth(), "Failure");

    const PlaneInfo::PlaneData& planeDataV = planeInfo.getPlaneData(1);
    QVERIFY2(planeDataV.pixels, "Failure");
    QVERIFY2(planeDataV.width == pVideoBuffer->getWidth() / 2, "Failure");
    QVERIFY2(planeDataV.height == pVideoBuffer->getHeight() / 2, "Failure");
    QVERIFY2(planeDataV.pitch == pVideoBuffer->getWidth() / 2, "Failure");

    const PlaneInfo::PlaneData& planeDataU = planeInfo.getPlaneData(2);
    QVERIFY2(planeDataU.pixels, "Failure");
    QVERIFY2(planeDataU.width == pVideoBuffer->getWidth() / 2, "Failure");
    QVERIFY2(planeDataU.height == pVideoBuffer->getHeight() / 2, "Failure");
    QVERIFY2(planeDataU.pitch == pVideoBuffer->getWidth() / 2, "Failure");

    pVideoBuffer->unlockPlane();

    pMediaBufferFactory->destroyVideoBuffer(pVideoBuffer);
}

void TestVideoBufferTest::testWrapperVideoBuffer()
{
    IMediaBufferFactory* pMediaBufferFactory =  surveon::mf::getSystemMemoryMediaBufferFactory();
    IVideoBuffer* pVideoBuffer =  pMediaBufferFactory->createVideoBuffer(1920, 1080, surveon::mf::SMF_VIDEO_FORMAT_YV12);

    // test wrapper video buffer
    WrapperVideoBuffer wrapperVideoBuffer(
                pVideoBuffer->getWidth(), pVideoBuffer->getHeight(), pVideoBuffer->getVideoFormat());

    const PlaneInfo& sourcePlaneInfo = pVideoBuffer->lockPlane();

    wrapperVideoBuffer.setBufferPtr(sourcePlaneInfo.getPlaneData(0).pixels);

    const PlaneInfo& planeInfo = wrapperVideoBuffer.lockPlane();
    QVERIFY2(planeInfo.getNumberOfPlanes() == 3, "Failure");

    const PlaneInfo::PlaneData& planeDataY = planeInfo.getPlaneData(0);
    QVERIFY2(planeDataY.pixels == sourcePlaneInfo.getPlaneData(0).pixels, "Failure");
    QVERIFY2(planeDataY.width == pVideoBuffer->getWidth(), "Failure");
    QVERIFY2(planeDataY.height == pVideoBuffer->getHeight(), "Failure");
    QVERIFY2(planeDataY.pitch == pVideoBuffer->getWidth(), "Failure");

    const PlaneInfo::PlaneData& planeDataV = planeInfo.getPlaneData(1);
    QVERIFY2(planeDataV.pixels == sourcePlaneInfo.getPlaneData(1).pixels, "Failure");
    QVERIFY2(planeDataV.width == pVideoBuffer->getWidth() / 2, "Failure");
    QVERIFY2(planeDataV.height == pVideoBuffer->getHeight() / 2, "Failure");
    QVERIFY2(planeDataV.pitch == pVideoBuffer->getWidth() / 2, "Failure");

    const PlaneInfo::PlaneData& planeDataU = planeInfo.getPlaneData(2);
    QVERIFY2(planeDataU.pixels == sourcePlaneInfo.getPlaneData(2).pixels, "Failure");
    QVERIFY2(planeDataU.width == pVideoBuffer->getWidth() / 2, "Failure");
    QVERIFY2(planeDataU.height == pVideoBuffer->getHeight() / 2, "Failure");
    QVERIFY2(planeDataU.pitch == pVideoBuffer->getWidth() / 2, "Failure");

    wrapperVideoBuffer.unlockPlane();

    pVideoBuffer->unlockPlane();

    pMediaBufferFactory->destroyVideoBuffer(pVideoBuffer);
}

QTEST_APPLESS_MAIN(TestVideoBufferTest)

#include "tst_TestVideoBufferTest.moc"
