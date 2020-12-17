
#include "SMFDXVA2HWAccel.h"
#include "../SMFFFmpegHelper.h"


namespace surveon
{
namespace mf
{


DXVA2HWAccel::DXVA2HWAccel(AVCodecContext* pContext,
                           IDirect3DDeviceManager9* pDeviceManager, HANDLE  deviceHandle,
                           IDirectXVideoDecoderService* pDXVDService, const GUID&  decoderGUID) :
    m_pAVContext(pContext),
    m_pDXVAContent(nullptr),
    m_pD3DDeviceManager(pDeviceManager),
    m_D3DDeviceHandle(deviceHandle),
    m_pDXVDService(pDXVDService),
    m_DecoderGUID(decoderGUID),
    m_RenderTargetFormat((D3DFORMAT)MKTAG('N','V','1','2')),
    m_SurfaceInfos(nullptr),
    m_SurfaceIndex(0)
{
    assert(pContext);
     assert(pDXVDService);

    m_pAVContext->opaque = this;   
}

DXVA2HWAccel::~DXVA2HWAccel(void)
{
    shutdown();

    m_pAVContext->opaque = nullptr;
}

void DXVA2HWAccel::initialize(void)
{
    SMF_FFMPEG_LOG_INFO("Initialize DXVA2 HWAccel");

    if (m_pAVContext->codec_id == AV_CODEC_ID_H264 &&
        (m_pAVContext->profile & ~FF_PROFILE_H264_CONSTRAINED) > FF_PROFILE_H264_HIGH)
    {
        SMF_FFMPEG_THROW(InternalErrorException,
                         "Unsupported H.264 profile for DXVA2 HWAccel: " << m_pAVContext->profile);
    }

    m_pDXVAContent = (dxva_context*)av_mallocz(sizeof(struct dxva_context));
    if(!m_pDXVAContent)
    {
        SMF_FFMPEG_THROW(InternalErrorException,  "Fail to allocate dxva_context" );
    }

    m_pAVContext->hwaccel_context = m_pDXVAContent;
}

void DXVA2HWAccel::shutdown(void)
{
    if(m_pDXVAContent)
    {
        SMF_FFMPEG_LOG_INFO("Shutdown DXVA2 HWAccel");

        shutdownDecoder();

        av_freep(m_pDXVAContent);

        m_pAVContext->hwaccel_context = nullptr;
    }
}


void DXVA2HWAccel::retriveVideoData(AVFrame* pFrame, IVideoBuffer* pVideoBuffer)
{
    LPDIRECT3DSURFACE9 pSourceSurface = (LPDIRECT3DSURFACE9)(uintptr_t)pFrame->data[3];
    assert(pSourceSurface);

    if(pVideoBuffer->getVideoFormat() == mf::SMF_VIDEO_FORMAT_YV12)
    {
        D3DLOCKED_RECT lockRect;
        HRESULT hr = pSourceSurface->LockRect(&lockRect, NULL, D3DLOCK_READONLY);

        const PlaneInfo& planInfo = pVideoBuffer->lockPlane();

        const PlaneInfo::PlaneData& planeY =  planInfo.getPlaneData(0);

        memcpy(planeY.pixels, lockRect.pBits, planeY.pitch * planeY.height);

        pVideoBuffer->unlockPlane();

        hr = pSourceSurface->UnlockRect();
    }
    else if(pVideoBuffer->getVideoFormat() == mf::SMF_VIDEO_FORMAT_D3D9_SURFACE)
    {
        IDirect3DDevice9 *pD3DDevice = NULL;
        HRESULT hr = m_pD3DDeviceManager->LockDevice(m_D3DDeviceHandle, &pD3DDevice, TRUE);
        SMF_DX_CHECK(hr, "Fail to lock D3D9 device");

        LPDIRECT3DSURFACE9 pDestSurface = (LPDIRECT3DSURFACE9)pVideoBuffer->getPrivateData();
        assert(pDestSurface);

        hr = pD3DDevice->StretchRect(pSourceSurface, NULL, pDestSurface, NULL, D3DTEXF_NONE);
        SMF_DX_CHECK(hr, "Fail to copy D3D9Surface");

        hr = m_pD3DDeviceManager->UnlockDevice(m_D3DDeviceHandle, TRUE);
        SMF_DX_CHECK(hr, "Fail to unlock D3D9 device");
    }
}

void DXVA2HWAccel::setDirectVideoOutTarget(mf::WindowHandle /*handle*/)
{

}

MediaSubType DXVA2HWAccel::getFormat(void) const
{
    return SMF_VIDEO_FORMAT_DXVA2;
}


int DXVA2HWAccel::getBuffer(AVFrame* pFrame, int /*flags*/)
{    
    LPDIRECT3DSURFACE9 pSurface = NULL;
    SurfaceInfo* pSurfaceInfo = NULL;
    for(unsigned i=0; i<m_pDXVAContent->surface_count; i++)
    {
        pSurfaceInfo = &m_SurfaceInfos[i];
        if(!pSurfaceInfo->used)
        {
            pSurfaceInfo->used = true;
            pSurface = m_pDXVAContent->surface[i];
            break;
        }
    }

    if(!pSurface)
    {
        SMF_FFMPEG_LOG_ERROR("No free surface");
        return AVERROR(ENOMEM);
    }

    pFrame->buf[0] = av_buffer_create((uint8_t*)&(pSurface),
                                     sizeof(LPDIRECT3DSURFACE9),
                                     DXVA2HWAccel::releaseBuffer, pSurfaceInfo,
                                     AV_BUFFER_FLAG_READONLY);

    if (!pFrame->buf[0])
    {
        SMF_FFMPEG_LOG_ERROR("Fail to create buf of AVFrame");
        return AVERROR(ENOMEM);
    }

    pFrame->data[3] = (uint8_t*)(uintptr_t)pSurface;
    return 0;
}

//===============================================================================

void DXVA2HWAccel::initDecoder(void)
{    
    SMF_FFMPEG_LOG_INFO("Create DXVA2 Decoder");

    DXVA2_VideoDesc desc = { 0 };
    desc.SampleWidth = m_pAVContext->coded_width;
    desc.SampleHeight = m_pAVContext->coded_height;
    desc.Format = m_RenderTargetFormat;

    getDecoderConfiguration(desc, m_DecoderConfig);

    int surface_alignment = 16;

    /* 4 base work m_pDXVAContent->surface */
    int num_surfaces = 6;

    /* add extra m_pDXVAContent->surface for frame threading */
    if (m_pAVContext->active_thread_type & FF_THREAD_FRAME)
        num_surfaces += m_pAVContext->thread_count;

    m_pDXVAContent->surface  = (LPDIRECT3DSURFACE9*)av_mallocz(num_surfaces * sizeof(LPDIRECT3DSURFACE9));
    m_SurfaceInfos = (SurfaceInfo*)av_mallocz(num_surfaces * sizeof(SurfaceInfo));

    if (!m_pDXVAContent->surface || !m_SurfaceInfos)
    {
        SMF_FFMPEG_THROW(InternalErrorException,  "Fail to allocate surface array" );
    }

    HRESULT hr = m_pDXVDService->CreateSurface(
                                                   FFALIGN(m_pAVContext->coded_width, surface_alignment),
                                                   FFALIGN(m_pAVContext->coded_height, surface_alignment),
                                                   num_surfaces-1,
                                                   m_RenderTargetFormat, D3DPOOL_DEFAULT,  0 ,
                                                   DXVA2_VideoDecoderRenderTarget,
                                                   m_pDXVAContent->surface, NULL);

    SMF_DX_CHECK(hr, "Failed to create video m_pDXVAContent->surface");

    hr = m_pDXVDService->CreateVideoDecoder(m_DecoderGUID,
                                                        &desc, &m_DecoderConfig, m_pDXVAContent->surface,
                                                        num_surfaces, &m_pDXVAContent->decoder);
    SMF_DX_CHECK(hr, "Failed to DXVA2 video decoder");

    m_pDXVAContent->cfg           = &m_DecoderConfig;
    m_pDXVAContent->surface_count = num_surfaces;

    if (IsEqualGUID(m_DecoderGUID, DXVADDI_Intel_ModeH264_E))
        m_pDXVAContent->workaround |= FF_DXVA2_WORKAROUND_INTEL_CLEARVIDEO;

}

void DXVA2HWAccel::shutdownDecoder(void)
{    
    if (m_pDXVAContent->surface)
    {
        for (uint i = 0; i < m_pDXVAContent->surface_count; i++)
        {
            if (m_pDXVAContent->surface[i])
                m_pDXVAContent->surface[i]->Release();
        }

        av_freep(m_pDXVAContent->surface);
        m_pDXVAContent->surface_count = 0;
    }

    if(m_SurfaceInfos)
        av_freep(&m_SurfaceInfos);

    SMF_DX_SAFE_RELEASE(m_pDXVAContent->decoder, "destroy DXVA decoeder")
}

//===============================================================================

void DXVA2HWAccel::getDecoderConfiguration(const DXVA2_VideoDesc& desc, DXVA2_ConfigPictureDecode& config)
{
    UINT configCount;
    DXVA2_ConfigPictureDecode* configList;
    HRESULT hr  = m_pDXVDService->GetDecoderConfigurations(m_DecoderGUID, &desc, NULL, &configCount, &configList);
    SMF_DX_CHECK(hr, "Fail to get decoder configurations");

    unsigned best_score = 0;
    DXVA2_ConfigPictureDecode* bestCfg = nullptr;

    for (UINT i = 0; i < configCount; i++)
    {
        DXVA2_ConfigPictureDecode *cfg = &configList[i];

        unsigned score=0;

        if (cfg->ConfigBitstreamRaw == 1)
            score = 1;
        else if (m_pAVContext->codec_id == AV_CODEC_ID_H264 && cfg->ConfigBitstreamRaw == 2)
            score = 2;
        else
            continue;

        if (IsEqualGUID(cfg->guidConfigBitstreamEncryption, DXVA2_NoEncrypt))
            score += 16;
        if (score > best_score)
        {
            best_score = score;
            bestCfg   = cfg;
        }
    }

    if(bestCfg)
    {
        config = *bestCfg;
        CoTaskMemFree (configList);
    }
    else
    {
        CoTaskMemFree (configList);
        SMF_FFMPEG_THROW(InternalErrorException,  "No valid decoder configuration available" );
    }    
}

//===============================================================================

void DXVA2HWAccel::releaseBuffer(void* opaque, uint8_t* /*data*/)
{
    SurfaceInfo* pSurfaceInfo = (SurfaceInfo*)opaque;
    assert(pSurfaceInfo);

    pSurfaceInfo->used = false;
}

} // namespace mf
} // namespace surveon

