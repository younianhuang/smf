
#include "SMFVDPAUDevice.h"


namespace surveon
{
namespace mf
{




VDPAUDevice::VDPAUDevice(void) :
    m_pXDisplay(nullptr),
    m_VdpDevice(0)
{

}

VDPAUDevice::~VDPAUDevice(void)
{
    shutdown();
}


void VDPAUDevice::initialize(void)
{
    SMF_FFMPEG_LOG_INFO("Create VdpDevice");

    m_pXDisplay = XOpenDisplay(0);
    if(!m_pXDisplay)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "fail to open XDisplay");
    }

    VdpStatus status;
    status = vdp_device_create_x11(m_pXDisplay, DefaultScreen(m_pXDisplay), &m_VdpDevice,
            &vdp_get_proc_address);
    if (status != VDP_STATUS_OK)
    {
        SMF_FFMPEG_THROW(InternalErrorException,"fail to create VdpDevice");
    }

    getProcAddress(VDP_FUNC_ID_DEVICE_DESTROY,
                                (void**)&vdp_device_destroy);

    getProcAddress(VDP_FUNC_ID_OUTPUT_SURFACE_CREATE,
            (void**)&vdp_output_surface_create);
    getProcAddress(VDP_FUNC_ID_OUTPUT_SURFACE_DESTROY,
            (void**)&vdp_output_surface_destroy);
    getProcAddress(VDP_FUNC_ID_OUTPUT_SURFACE_GET_BITS_NATIVE,
            (void**)&vdp_output_surface_get_bits_native);
    getProcAddress(VDP_FUNC_ID_OUTPUT_SURFACE_PUT_BITS_NATIVE,
            (void**)&vdp_output_surface_put_bits_native);
    getProcAddress(VDP_FUNC_ID_VIDEO_SURFACE_CREATE,
            (void**)&vdp_video_surface_create);
    getProcAddress(VDP_FUNC_ID_VIDEO_SURFACE_DESTROY,
            (void**)&vdp_video_surface_destroy);
    getProcAddress(VDP_FUNC_ID_DECODER_CREATE, (void**)&vdp_decoder_create);
    getProcAddress(VDP_FUNC_ID_DECODER_DESTROY, (void**)&vdp_decoder_destroy);
    getProcAddress(VDP_FUNC_ID_DECODER_RENDER, (void**)&vdp_decoder_render);
    getProcAddress(VDP_FUNC_ID_VIDEO_SURFACE_GET_BITS_Y_CB_CR,
            (void**)&vdp_video_surface_get_bits_y_cb_cr);
    getProcAddress(VDP_FUNC_ID_VIDEO_MIXER_CREATE,
            (void**)&vdp_video_mixer_create);
    getProcAddress(VDP_FUNC_ID_VIDEO_MIXER_DESTROY,
            (void**)&vdp_video_mixer_destroy);
    getProcAddress(VDP_FUNC_ID_VIDEO_MIXER_RENDER,
            (void**)&vdp_video_mixer_render);
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_CREATE,
            (void**)&vdp_presentation_queue_create);
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_DESTROY,
            (void**)&vdp_presentation_queue_destroy);
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_CREATE_X11,
            (void**)&vdp_presentation_queue_target_create_x11);    
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_TARGET_DESTROY,
            (void**)&vdp_presentation_queue_target_destroy);
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_QUERY_SURFACE_STATUS,
            (void**)&vdp_presentation_queue_query_surface_status);
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_DISPLAY,
            (void**)&vdp_presentation_queue_display);
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_GET_TIME,
            (void**)&vdp_presentation_queue_get_time);
    getProcAddress(VDP_FUNC_ID_PRESENTATION_QUEUE_BLOCK_UNTIL_SURFACE_IDLE,
            (void**)&vdp_presentation_queue_block_until_surface_idle);
    getProcAddress(VDP_FUNC_ID_VIDEO_SURFACE_GET_PARAMETERS,
            (void**)&vdp_video_surface_get_parameters);
    getProcAddress(VDP_FUNC_ID_OUTPUT_SURFACE_GET_PARAMETERS,
            (void**)&vdp_output_surface_get_parameters);

    getProcAddress(VDP_FUNC_ID_GET_API_VERSION,
            (void**)&vdp_get_api_version);
    getProcAddress(VDP_FUNC_ID_GET_INFORMATION_STRING,
            (void**)&vdp_get_information_string);   
    getProcAddress(VDP_FUNC_ID_GET_ERROR_STRING,
           (void**)&vdp_get_error_string);




    uint32_t version = 0;
    if(vdp_get_api_version(&version) == VDP_STATUS_OK)
    {
        SMF_FFMPEG_LOG_INFO("VDPAU verison " << version);
    }

    const char* info = nullptr;
    if(vdp_get_information_string(&info) == VDP_STATUS_OK)
    {
        SMF_FFMPEG_LOG_INFO(info);
    }
}

void VDPAUDevice::shutdown(void)
{
    if(m_VdpDevice)
    {
        SMF_FFMPEG_LOG_INFO("Destroy VdpDevice");

        m_VDPAUDecoderFactory.destroyAll();

        vdp_device_destroy(m_VdpDevice);

        m_VdpDevice = 0;
    }

    if(m_pXDisplay)
    {
        XCloseDisplay(m_pXDisplay);

        m_pXDisplay = nullptr;
    }
}



HWAcceleration* VDPAUDevice::createHWAccel(AVCodecContext* pContext)
{
    assert(pContext);
    assert(!pContext->opaque);

    VDPAUHWAccel* pHWAccel = m_VDPAUDecoderFactory.create(pContext, m_VdpDevice);

    pHWAccel->initialize();

    pContext->opaque = pHWAccel;

    pContext->get_format = getFormat;
    pContext->get_buffer2 = getBuffer;
    //pContext->thread_safe_callbacks = 1;
    //pContext->draw_horiz_band = drawHorizBand;

    return pHWAccel;
}

void VDPAUDevice::destroyHWAccel(struct AVCodecContext* pContext)
{
    assert(pContext);

    VDPAUHWAccel* pHWAccel = (VDPAUHWAccel*)pContext->opaque;
    assert(pHWAccel);

    m_VDPAUDecoderFactory.destroy(pHWAccel);

    pContext->opaque = nullptr;
}


//==========================================================================

void VDPAUDevice::getProcAddress(VdpFuncId functionId, void** functionPointer)
{
    VdpStatus status = vdp_get_proc_address(m_VdpDevice, functionId, functionPointer);
    if(status != VDP_STATUS_OK)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "fail to get address of proc:" <<  functionId);
    }
}

//=================================================================================

enum AVPixelFormat VDPAUDevice::getFormat(AVCodecContext *pContext, const enum AVPixelFormat *pix_fmts)
{
    for(const AVPixelFormat* fmt = pix_fmts; *fmt != AV_PIX_FMT_NONE; fmt++)
    {
        if(*fmt == AV_PIX_FMT_VDPAU)
        {
            return AV_PIX_FMT_VDPAU;
        }
    }

    return AV_PIX_FMT_NONE;
}

int VDPAUDevice::getBuffer(AVCodecContext* pContext, AVFrame* pFrame, int flags)
{
    VDPAUHWAccel* pHWAccel = (VDPAUHWAccel*)pContext->opaque;
    assert(pHWAccel);

    return pHWAccel->getBuffer(pFrame, flags);
}

/*
void VDPAUDevice::drawHorizBand(struct AVCodecContext* pContext, const AVFrame* pSrcFrame,
            int offset[4], int y, int type, int height)
{
    VDPAUDecoder* pHWAccel = (VDPAUDecoder*)pContext->opaque;
    assert(pHWAccel);

    pHWAccel->drawHorizBand(pSrcFrame);
}
*/

} // namespace mf
} // namespace surveon
