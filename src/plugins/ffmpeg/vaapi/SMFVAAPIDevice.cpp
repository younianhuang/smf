#include "SMFVAAPIDevice.h"



namespace surveon
{
namespace mf
{


VAAPIDevice::VAAPIDevice(void) :
    m_pXDisplay(nullptr),
    m_VADisplay(0)
{

}

VAAPIDevice::~VAAPIDevice(void)
{
    shutdown();
}


void VAAPIDevice::initialize(void)
{
    SMF_FFMPEG_LOG_INFO("Initialize vaapi device");

    m_pXDisplay = XOpenDisplay(NULL);
    if(!m_pXDisplay)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "fail to open XDisplay");
    }

    m_VADisplay = vaGetDisplay(m_pXDisplay);
    if(m_VADisplay == 0)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "fail to open VADisplay");
    }


    SMF_FFMPEG_LOG_INFO("Initialize vaapi");

    int major_version = 0;
    int minor_version = 0;

    SMF_VAAPI_CHECK(vaInitialize(m_VADisplay, &major_version, &minor_version));

    SMF_FFMPEG_LOG_INFO("vaapi version " << major_version <<"." << minor_version);
}



void VAAPIDevice::shutdown(void)
{
    if(m_VADisplay)
    {
        SMF_FFMPEG_LOG_INFO("Shutdown vaapi device");

        SMF_VAAPI_TRACE(vaTerminate(m_VADisplay));

        m_VADisplay = 0;
    }

    if(m_pXDisplay)
    {
        XCloseDisplay(m_pXDisplay);

        m_pXDisplay = nullptr;
    }
}



HWAcceleration* VAAPIDevice::createHWAccel(AVCodecContext* pContext)
{
    assert(pContext);
    assert(!pContext->opaque);    

    SMF_FFMPEG_LOG_INFO("create vaapi hardware accelerator");

    VAAPIHWAccel* pHWAccel = m_VDPAUDecoderFactory.create(pContext, m_pXDisplay, m_VADisplay);

    pHWAccel->initialize();

    pContext->get_format = VAAPIDevice::getFormat;
    pContext->get_buffer2 = VAAPIDevice::getBuffer;
    //pContext->thread_safe_callbacks = 1;
    //pContext->draw_horiz_band = drawHorizBand;

    return pHWAccel;
}

void VAAPIDevice::destroyHWAccel(struct AVCodecContext* pContext)
{
    assert(pContext);

    SMF_FFMPEG_LOG_INFO("destroy vaapi hardware accelerator");

    VAAPIHWAccel* pHWAccel = (VAAPIHWAccel*)pContext->opaque;
    assert(pHWAccel);

    m_VDPAUDecoderFactory.destroy(pHWAccel);

    //pContext->opaque = nullptr;
}


//===============================================================================

enum AVPixelFormat VAAPIDevice::getFormat(AVCodecContext* pContext, const enum AVPixelFormat *pix_fmts)
{
    try
    {
        VAAPIHWAccel* pHWAccel = (VAAPIHWAccel*)pContext->opaque;
        if(pHWAccel)
        {
            pHWAccel->initDecoder();
        }

        for (int i = 0; pix_fmts[i] != AV_PIX_FMT_NONE; i++)
        {
            if (pix_fmts[i] == AV_PIX_FMT_VAAPI_VLD)
                return AV_PIX_FMT_VAAPI_VLD;
        }

        return AV_PIX_FMT_NONE;
    }
    catch(std::exception& ex)
    {
        return AV_PIX_FMT_NONE;
    }   
}

int VAAPIDevice::getBuffer(AVCodecContext* pContext, AVFrame* pFrame, int flags)
{
    VAAPIHWAccel* pHWAccel = (VAAPIHWAccel*)pContext->opaque;
    assert(pHWAccel);

    return pHWAccel->getBuffer(pFrame, flags);
}



} // namespace mf
} // namespace surveon


