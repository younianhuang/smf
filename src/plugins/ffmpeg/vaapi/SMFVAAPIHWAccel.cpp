
#include "SMFVAAPIHWAccel.h"
#include "../SMFFFmpegHelper.h"


namespace surveon
{
namespace mf
{


VAAPIHWAccel::VAAPIHWAccel(AVCodecContext* pContext, Display* pDisplay, VADisplay vaDisplay) :
    m_pAVContext(pContext),
    m_pVAContext(nullptr),
    m_pXDisplay(pDisplay),
    m_VADisplay(vaDisplay),
    m_Drawable(0),
    m_WinWidth(0),
    m_WinHeight(0),
    m_VASurfaces{VA_INVALID_SURFACE},
    m_SurfaceIndex(0)
{
    assert(pContext);
    assert(vaDisplay);

    m_pAVContext->opaque = this;   
}

VAAPIHWAccel::~VAAPIHWAccel(void)
{
    shutdown();

    m_pAVContext->opaque = nullptr;
}

void VAAPIHWAccel::initialize(void)
{
    assert(m_pVAContext == nullptr);

    m_pVAContext = (vaapi_context*)av_malloc(sizeof(vaapi_context));
    memset(m_pVAContext, 0, sizeof(vaapi_context));

    m_pVAContext->display = m_VADisplay;

    m_pVAContext->context_id = VA_INVALID_ID;
    m_pVAContext->config_id = VA_INVALID_ID;

    m_pAVContext->hwaccel_context = m_pVAContext;
}

void VAAPIHWAccel::shutdown(void)
{
    if(m_pVAContext)
    {
        shutdownDecoder();

        m_pVAContext->display = 0;

        av_freep(m_pVAContext);

        m_pAVContext->hwaccel_context = nullptr;
    }
}


void VAAPIHWAccel::retriveVideoData(AVFrame* pFrame, IVideoBuffer* pVideoBuffer)
{
    VASurfaceID surface = (VASurfaceID)(uintptr_t)pFrame->data[3];

    if(m_Drawable)
    {        
        if(m_WinWidth == 0 || m_WinHeight == 0)
        {
            XWindowAttributes winAttrs;
            XGetWindowAttributes(m_pXDisplay, m_Drawable, &winAttrs);

            m_WinWidth = winAttrs.width;
            m_WinHeight = winAttrs.height;
        }

        SMF_VAAPI_CHECK(vaPutSurface(
                        m_VADisplay, surface, m_Drawable,
                        0, 0, m_pAVContext->coded_width, m_pAVContext->coded_height,
                        0, 0, m_WinWidth, m_WinHeight,
                        NULL, 0,
                        VA_FRAME_PICTURE));

        //std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    else if(pVideoBuffer->getVideoFormat() == mf::SMF_VIDEO_FORMAT_XPIXMAP)
    {
        Drawable drawable = (Drawable)pVideoBuffer->getPrivateData();

        SMF_VAAPI_CHECK(vaPutSurface(
                    m_VADisplay, surface, drawable,
                    0, 0, m_pAVContext->coded_width, m_pAVContext->coded_height,
                    0, 0, pVideoBuffer->getWidth(), pVideoBuffer->getHeight(),
                    NULL, 0,
                    VA_FRAME_PICTURE));
    }
}

void VAAPIHWAccel::setDirectVideoOutTarget(mf::WindowHandle handle)
{
    m_Drawable = handle;    
}

MediaSubType VAAPIHWAccel::getFormat(void) const
{
    return SMF_VIDEO_FORMAT_VAAPI;
}


int VAAPIHWAccel::getBuffer(AVFrame* pFrame, int /*flags*/)
{    
/*
    if(m_pVAContext->context_id == VA_INVALID_ID)
    {
        initDecoder();
    }
*/
    RenderTargetState* pRenderTarget = getFreeRenderTarget();
    if(!pRenderTarget)
    {
        SMF_FFMPEG_LOG_ERROR("No free vaSurface");
        return AVERROR(ENOMEM);
    }

    pFrame->buf[0] = av_buffer_create((uint8_t*)&(pRenderTarget->vaSurface),
                                      sizeof(VASurfaceID),
                                     VAAPIHWAccel::releaseBuffer, pRenderTarget,
                                     AV_BUFFER_FLAG_READONLY);

    if (!pFrame->buf[0])
    {
        SMF_FFMPEG_LOG_ERROR("Fail to create buf of AVFrame");
        return AVERROR(ENOMEM);
    }

    pFrame->data[3] = (uint8_t*)(uintptr_t)pRenderTarget->vaSurface;// surface;

    return 0;
}

//===============================================================================
void VAAPIHWAccel::initDecoder(void)
{
    SMF_FFMPEG_LOG_INFO("create vaapi context");    

    VAProfile profile = VAProfileNone;
    switch (m_pAVContext->codec_id)
    {
    case AV_CODEC_ID_MPEG2VIDEO:
        profile = VAProfileMPEG2Main;
        break;
    case AV_CODEC_ID_MPEG4:
    case AV_CODEC_ID_H263:
        profile = VAProfileMPEG4AdvancedSimple;
        break;
    case AV_CODEC_ID_H264:
        profile = VAProfileH264High;
        break;
    case AV_CODEC_ID_WMV3:
        profile = VAProfileVC1Main;
        break;
    case AV_CODEC_ID_VC1:
        profile = VAProfileVC1Advanced;
        break;
    default:
        assert(profile != VAProfileNone);
        break;
    }

    VAConfigAttrib attrib;
    attrib.type = VAConfigAttribRTFormat;

    VAEntrypoint entryPoint = VAEntrypointVLD;

    SMF_VAAPI_CHECK(vaGetConfigAttributes(m_VADisplay, profile, entryPoint, &attrib, 1));
    SMF_FFMPEG_LOG_INFO("vaapi RenderTarget support format " << attrib.value);

    int rtFormat = chooseRTFormat(attrib.value);

    SMF_VAAPI_CHECK(vaCreateConfig(m_VADisplay, profile, entryPoint, &attrib, 1, &m_pVAContext->config_id));

    SMF_VAAPI_CHECK(vaCreateSurfaces(m_VADisplay, rtFormat,
                                     m_pAVContext->coded_width, m_pAVContext->coded_height,
                                     &m_VASurfaces[0], VA_NUM_OF_RENDER_TARGETS,
                                     NULL, 0));

    SMF_VAAPI_CHECK(vaCreateContext(m_VADisplay, m_pVAContext->config_id,
                             m_pAVContext->coded_width, m_pAVContext->coded_height,
                             VA_PROGRESSIVE,
                             &m_VASurfaces[0], VA_NUM_OF_RENDER_TARGETS,
                             &m_pVAContext->context_id));

    for(size_t i=0; i<VA_NUM_OF_RENDER_TARGETS; i++)
    {
        RenderTargetState& renderTarget = m_RenderTargetStates[i];
        renderTarget.vaSurface = m_VASurfaces[i];
        renderTarget.used = false;
    }
}

void VAAPIHWAccel::shutdownDecoder(void)
{
    if (m_pVAContext->context_id != VA_INVALID_ID)
    {
        SMF_FFMPEG_LOG_INFO("destroy vaapi context");

        SMF_VAAPI_TRACE(vaDestroyContext(m_VADisplay, m_pVAContext->context_id));
        m_pVAContext->context_id = VA_INVALID_ID;
    }

    if(m_VASurfaces[0] != VA_INVALID_SURFACE)
    {
        SMF_VAAPI_TRACE(vaDestroySurfaces(m_VADisplay, m_VASurfaces, VA_NUM_OF_RENDER_TARGETS));


        for(int i=0; i<VA_NUM_OF_RENDER_TARGETS; i++)
        {            
            m_VASurfaces[i] = VA_INVALID_ID;

            RenderTargetState& renderTarget = m_RenderTargetStates[i];
            renderTarget.vaSurface = VA_INVALID_ID;
            renderTarget.used = false;
        }
    }

    if (m_pVAContext->config_id != VA_INVALID_ID)
    {
        SMF_VAAPI_TRACE(vaDestroyConfig(m_VADisplay, m_pVAContext->config_id));
        m_pVAContext->config_id = VA_INVALID_ID;
    }
}

int VAAPIHWAccel::chooseRTFormat(int supportFormat)
{
    int rtFormat = 0;
    if(supportFormat == 0)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "vaapi no any RenderTarget Format support");
    }
    else if(supportFormat & VA_RT_FORMAT_YUV420)
    {
        rtFormat = VA_RT_FORMAT_YUV420;
        SMF_FFMPEG_LOG_INFO("vaapi select RenderTarget format VA_RT_FORMAT_YUV420");
    }
    else if(supportFormat & VA_RT_FORMAT_YUV422)
    {
        rtFormat = VA_RT_FORMAT_YUV422;
        SMF_FFMPEG_LOG_INFO("vaapi select RenderTarget format VA_RT_FORMAT_YUV422");
    }
    else if(supportFormat & VA_RT_FORMAT_YUV444)
    {
        rtFormat = VA_RT_FORMAT_YUV444;
        SMF_FFMPEG_LOG_INFO("vaapi select RenderTarget format VA_RT_FORMAT_YUV444");
    }
    else if(supportFormat & VA_RT_FORMAT_YUV411)
    {
        rtFormat = VA_RT_FORMAT_YUV411;
        SMF_FFMPEG_LOG_INFO("vaapi select RenderTarget format VA_RT_FORMAT_YUV411");
    }
    else if(supportFormat & VA_RT_FORMAT_YUV400)
    {
        rtFormat = VA_RT_FORMAT_YUV400;
        SMF_FFMPEG_LOG_INFO("vaapi select RenderTarget format VA_RT_FORMAT_YUV400");
    }
    else if(supportFormat & VA_RT_FORMAT_RGB16)
    {
        rtFormat = VA_RT_FORMAT_RGB16;
        SMF_FFMPEG_LOG_INFO("vaapi select RenderTarget format VA_RT_FORMAT_RGB16");
    }
    else if(supportFormat & VA_RT_FORMAT_RGB32)
    {
        rtFormat = VA_RT_FORMAT_RGB32;
        SMF_FFMPEG_LOG_INFO("vaapi select RenderTarget format VA_RT_FORMAT_RGB32");
    }

    return rtFormat;
}


VAAPIHWAccel::RenderTargetState* VAAPIHWAccel::getFreeRenderTarget(void)
{
    for(int i=0; i<VA_NUM_OF_RENDER_TARGETS; i++)
    {
        RenderTargetState* pRenderTarget = &m_RenderTargetStates[i];

        if(!pRenderTarget->used)
        {
            pRenderTarget->used = true;
            return pRenderTarget;
        }
    }

    return nullptr;
}


//===============================================================================

void VAAPIHWAccel::releaseBuffer(void* opaque, uint8_t* /*data*/)
{
    RenderTargetState* pRenderTarget = (RenderTargetState*)opaque;

    pRenderTarget->used = false;
}

} // namespace mf
} // namespace surveon

