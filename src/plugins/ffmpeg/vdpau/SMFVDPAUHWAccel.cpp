#include "SMFVDPAUHWAccel.h"
#include "../SMFFFmpegHelper.h"


namespace surveon
{
namespace mf
{


VDPAUHWAccel::VDPAUHWAccel(AVCodecContext* pContext, VdpDevice device) :
    m_pAVCodecContext(pContext),
    m_VdpDevice(device),
    m_VdpDecoder(VDP_INVALID_HANDLE),
    m_VdpVideoMixer(VDP_INVALID_HANDLE),
    m_OutputSurface(VDP_INVALID_HANDLE),
    m_VdpPresentationQueue(VDP_INVALID_HANDLE),
    m_VdpPresentationQueueTarget(VDP_INVALID_HANDLE),
    m_Drawable(0),
    m_InputWidth(0),
    m_InputHeight(0),
    m_InputChroma(0)
{
    assert(pContext);
    assert(device);   
}

VDPAUHWAccel::~VDPAUHWAccel(void)
{
    close();
    av_freep(&m_pAVCodecContext->hwaccel_context);
}

void VDPAUHWAccel::initialize(void)
{
    if(av_vdpau_bind_context(m_pAVCodecContext,
                                 m_VdpDevice, vdp_get_proc_address,0))
    {
        SMF_FFMPEG_THROW(InternalErrorException,
                         "fail to bind VDPAU Contex");
    }
}

void VDPAUHWAccel::shutdown(void)
{
    close();
    av_freep(&m_pAVCodecContext->hwaccel_context);
}


void VDPAUHWAccel::retriveVideoData(AVFrame* pFrame, IVideoBuffer* pVideoBuffer)
{
    VdpVideoSurface surface = (VdpVideoSurface)(uintptr_t)pFrame->data[3];
    assert(surface);

    assert(pVideoBuffer->getVideoFormat() == MediaSubType::SMF_VIDEO_FORMAT_RGBA);

    const PlaneInfo&  planInfo = pVideoBuffer->lockPlane();

    PlaneInfo::PlaneData planData =  planInfo.getPlaneData(0);

    if(m_OutputSurface == VDP_INVALID_HANDLE)
    {
        SMF_VDPAU_CHECK(vdp_output_surface_create(
                            m_VdpDevice, VDP_RGBA_FORMAT_R8G8B8A8,
                            planData.width, planData.height,
                            &m_OutputSurface));

        VdpRGBAFormat format;
        uint32_t width, height;

        SMF_VDPAU_CHECK(vdp_output_surface_get_parameters(
                            m_OutputSurface, &format, &width, &height));
    }

    VdpRect destRect = {0, 0, planData.width, planData.height};

    SMF_VDPAU_CHECK(
            vdp_video_mixer_render(m_VdpVideoMixer,
            VDP_INVALID_HANDLE,
            nullptr,
            VDP_VIDEO_MIXER_PICTURE_STRUCTURE_FRAME,
            0, nullptr,
            //pRenderState->surface,
            surface,
            0, nullptr,
            nullptr,
            m_OutputSurface,
            nullptr, &destRect, 0, nullptr));

    uint8* dataArray[] = { planData.pixels};
    uint32_t pitchArray[] = { planData.pitch};

    if(m_VdpPresentationQueue != VDP_INVALID_HANDLE)
    {
        vdp_presentation_queue_display(m_VdpPresentationQueue, m_OutputSurface, 0, 0, 0);
    }
    else
    {
        SMF_VDPAU_CHECK(vdp_output_surface_get_bits_native(
                            m_OutputSurface, NULL,
                            (void* const*)dataArray,
                            (const uint32_t*)pitchArray));
    }

    pVideoBuffer->unlockPlane();
}

void VDPAUHWAccel::setDirectVideoOutTarget(mf::WindowHandle handle)
{
    m_Drawable = (Drawable)handle;
}

MediaSubType VDPAUHWAccel::getFormat(void) const
{
    return SMF_VIDEO_FORMAT_VDPAU;
}


void VDPAUHWAccel::open(void)
{
    SMF_FFMPEG_LOG_INFO("Create VdpDecoder");

    //createVdpDecoder();

    createVdpVideoMixer();

    createVdpPresentationQueue();
}



void VDPAUHWAccel::close(void)
{        
    destroyVdpVideoMixer();

    //destroyVdpDecoder();

    destroyVdpPresentationQueue();

    destroyAllRenderStates();
}


//===============================================================================

void VDPAUHWAccel::getSurfaceParameters(void)
{    
    if (av_vdpau_get_surface_parameters(m_pAVCodecContext, &m_InputChroma, &m_InputWidth, &m_InputHeight))
    {
        SMF_FFMPEG_THROW(InternalErrorException, "fail to get surface parameters");
    }
}

/*
void VDPAUDecoder::createVdpDecoder(void)
{
    // TODO Query support surface format:
    m_pAVCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    VdpDecoderProfile profile = 0;
    switch (m_pAVCodecContext->codec_id)
    {
        case AV_CODEC_ID_H264:
            profile = VDP_DECODER_PROFILE_H264_HIGH;
            break;
        default:
            SMF_FFMPEG_THROW(InternalErrorException, "unsupport pixformat:" << m_pAVCodecContext->pix_fmt);
    }

    SMF_VDPAU_CHECK(vdp_decoder_create(m_VdpDevice, profile,
                                       m_InputWidth, m_InputHeight, 16,
                                       &m_VdpDecoder));
}

void VDPAUDecoder::destroyVdpDecoder(void)
{
    if(m_VdpDecoder != VDP_INVALID_HANDLE)
    {
        SMF_FFMPEG_LOG_INFO("Destroy VdpDecoder");

        SMF_VDPAU_TRACE(vdp_decoder_destroy(m_VdpDecoder));
    }
}
*/

void VDPAUHWAccel::createVdpVideoMixer(void)
{
    VdpVideoMixerFeature features[] = {
        VDP_VIDEO_MIXER_FEATURE_HIGH_QUALITY_SCALING_L1,
        VDP_VIDEO_MIXER_FEATURE_HIGH_QUALITY_SCALING_L2,
    };
    VdpVideoMixerParameter params[] = {
        VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_WIDTH,
        VDP_VIDEO_MIXER_PARAMETER_VIDEO_SURFACE_HEIGHT,
        VDP_VIDEO_MIXER_PARAMETER_CHROMA_TYPE,
        VDP_VIDEO_MIXER_PARAMETER_LAYERS
    };

    int  numLayers = 0;
    void const* paramValues [] = { &m_InputWidth, &m_InputHeight, &m_InputChroma, &numLayers };

    SMF_VDPAU_CHECK(vdp_video_mixer_create(m_VdpDevice, 2, features, 4, params,
            paramValues, &m_VdpVideoMixer));
}

void VDPAUHWAccel::destroyVdpVideoMixer(void)
{
    if (m_VdpVideoMixer != VDP_INVALID_HANDLE)
    {
        SMF_VDPAU_TRACE(vdp_video_mixer_destroy(m_VdpVideoMixer));
    }

    if(m_OutputSurface != VDP_INVALID_HANDLE)
    {
        SMF_VDPAU_TRACE(vdp_output_surface_destroy(m_OutputSurface));
    }
}


void VDPAUHWAccel::createVdpPresentationQueue(void)
{
    if(m_Drawable)
    {
        SMF_VDPAU_CHECK(vdp_presentation_queue_target_create_x11(
                            m_VdpDevice, m_Drawable, &m_VdpPresentationQueueTarget));

        SMF_VDPAU_CHECK(vdp_presentation_queue_create(
                            m_VdpDevice, m_VdpPresentationQueueTarget, &m_VdpPresentationQueue));
    }
}

void VDPAUHWAccel::destroyVdpPresentationQueue(void)
{
    if(m_VdpPresentationQueue != VDP_INVALID_HANDLE)
    {
        SMF_VDPAU_CHECK(vdp_presentation_queue_destroy(m_VdpPresentationQueue));

        SMF_VDPAU_CHECK(vdp_presentation_queue_target_destroy(m_VdpPresentationQueueTarget));
    }
}


vdpau_render_state* VDPAUHWAccel::getFreeRenderState(void)
{
    for (size_t i = 0; i < m_RenderStates.size(); i++)
    {
        vdpau_render_state* pRenderState = m_RenderStates[i];
        if (!(pRenderState->state & FF_VDPAU_STATE_USED_FOR_REFERENCE))
        {
            return pRenderState;
        }
    }

    // No free surfaces available -> create new surface
    vdpau_render_state* pRenderState =
            (vdpau_render_state*)av_malloc(sizeof(vdpau_render_state));

    memset(pRenderState, 0, sizeof(vdpau_render_state));
    pRenderState->surface = VDP_INVALID_HANDLE;

    m_RenderStates.push_back(pRenderState);

    VdpStatus err = vdp_video_surface_create(m_VdpDevice,
                                   m_InputChroma, m_InputWidth, m_InputHeight,
                                   &(pRenderState->surface));
    if (err != VDP_STATUS_OK)
    {
        SMF_FFMPEG_LOG_ERROR("Error create a VDPAU video surface: " <<
                             vdp_get_error_string(err));
        return nullptr;
    }


    return pRenderState;
}

void VDPAUHWAccel::destroyAllRenderStates(void)
{
    for (size_t i = 0; i < m_RenderStates.size(); i++)
    {
        vdpau_render_state* pRenderState = m_RenderStates[i];

        vdp_video_surface_destroy(pRenderState->surface);
        av_free(pRenderState);
    }

    m_RenderStates.clear();
}


//===============================================================================

void VDPAUHWAccel::releaseBuffer(void* opaque, uint8_t *data)
{  
    //vdpau_render_state* pRenderState = (vdpau_render_state*)data;
    vdpau_render_state* pRenderState = (vdpau_render_state*)opaque;

    pRenderState->state &= ~FF_VDPAU_STATE_USED_FOR_REFERENCE;
}

int VDPAUHWAccel::getBuffer(AVFrame* pFrame, int flags)
{
    if(m_InputWidth == 0)
    {
        getSurfaceParameters();
        open();
    }

    vdpau_render_state* pRenderState = getFreeRenderState();

    if (!pRenderState)
        return AVERROR(ENOMEM);

    pRenderState->state |= FF_VDPAU_STATE_USED_FOR_REFERENCE;

    pFrame->buf[0] = av_buffer_create((uint8_t*)&(pRenderState->surface),
                                      sizeof(VdpVideoSurface),
                                     VDPAUHWAccel::releaseBuffer, pRenderState,
                                     AV_BUFFER_FLAG_READONLY);

    if (!pFrame->buf[0])
    {
        SMF_FFMPEG_LOG_ERROR("Fail to create buf of AVFrame");
        return AVERROR(ENOMEM);
    }

    pFrame->data[3] = (uint8_t*)pRenderState->surface;

    return 0;
}


// main rendering routine
/*
void VDPAUDecoder::drawHorizBand(const AVFrame* pSrcFrame)
{
    struct vdpau_render_state* pRenderState = (struct vdpau_render_state*)pSrcFrame->data[3];
    assert(pRenderState);

    //if(m_VdpDecoder == VDP_INVALID_HANDLE)
    //    open();

    VdpStatus status = vdp_decoder_render(m_VdpDecoder, pRenderState->surface,
            (VdpPictureInfo const*)&(pRenderState->info),
            pRenderState->bitstream_buffers_used, pRenderState->bitstream_buffers);

    if(status !=VDP_STATUS_OK)
    {
        SMF_FFMPEG_LOG_WARN("Fail to draw horizontal band:" << status);
    }
}


void VDPAUHWAccel::retriveVideoData(AVFrame* pFrame, IMediaBuffer* pMediaBuffer)
{   
    VdpVideoSurface surface = (VdpVideoSurface)(uintptr_t)pFrame->data[3];
    assert(surface);

    IVideoBuffer* pVideoBuffer = static_cast<IVideoBuffer*>(pMediaBuffer);
    assert(pVideoBuffer->getVideoFormat() == MediaSubType::SMF_VIDEO_FORMAT_RGBA);

    const PlaneInfo&  planInfo = pVideoBuffer->lockPlane();

    PlaneInfo::PlaneData planData =  planInfo.getPlaneData(0);

    if(m_OutputSurface == VDP_INVALID_HANDLE)
    {
        SMF_VDPAU_CHECK(vdp_output_surface_create(
                            m_VdpDevice, VDP_RGBA_FORMAT_R8G8B8A8,
                            planData.width, planData.height,
                            &m_OutputSurface));

        VdpRGBAFormat format;
        uint32_t width, height;

        SMF_VDPAU_CHECK(vdp_output_surface_get_parameters(
                            m_OutputSurface, &format, &width, &height));
    }

    VdpRect destRect = {0, 0, planData.width, planData.height};

    SMF_VDPAU_CHECK(
            vdp_video_mixer_render(m_VdpVideoMixer,
            VDP_INVALID_HANDLE,
            nullptr,
            VDP_VIDEO_MIXER_PICTURE_STRUCTURE_FRAME,
            0, nullptr,
            //pRenderState->surface,
            surface,
            0, nullptr,
            nullptr,
            m_OutputSurface,
            nullptr, &destRect, 0, nullptr));

    uint8* dataArray[] = { planData.pixels};
    uint32_t pitchArray[] = { planData.pitch};

    if(m_VdpPresentationQueue != VDP_INVALID_HANDLE)
    {
        vdp_presentation_queue_display(m_VdpPresentationQueue, m_OutputSurface, 0, 0, 0);
    }
    else
    {
        SMF_VDPAU_CHECK(vdp_output_surface_get_bits_native(
                            m_OutputSurface, NULL,
                            (void* const*)dataArray,
                            (const uint32_t*)pitchArray));
    }

    pVideoBuffer->unlockPlane();

}

*/


} // namespace mf
} // namespace surveon

