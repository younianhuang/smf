#include "SMFDXVA2Device.h"


namespace surveon
{
namespace mf
{

static const GUID* g_DeviceGUIDs [] =
{   &DXVA2_ModeH264_F,
    &DXVA2_ModeH264_E,
    /* Intel specific H.264 mode */
    &DXVADDI_Intel_ModeH264_E,
    NULL
};

DXVA2Device::DXVA2Device(IDeviceManager* pDeviceManager) :
    m_pD3DDeviceManager((IDirect3DDeviceManager9*)pDeviceManager->getNativeDeviceHandle()),
    m_pDXVDService(NULL),
    m_D3DDeviceHandle(NULL),
    m_DeviceGuidList(NULL),
    m_DeviceGuidCount(0)
{

}

DXVA2Device::~DXVA2Device(void)
{
    shutdown();
}

void DXVA2Device::initialize(void)
{    
    SMF_FFMPEG_LOG_INFO("Initialize DXVA2 device");

    HRESULT hr = m_pD3DDeviceManager->OpenDeviceHandle(&m_D3DDeviceHandle);
    SMF_DX_CHECK(hr, "Fail to  open d3d device handle");

    hr =  m_pD3DDeviceManager->GetVideoService(m_D3DDeviceHandle, IID_IDirectXVideoDecoderService, (void **) &m_pDXVDService);
    SMF_DX_CHECK(hr, "Failed to create IDirectXVideoDecoderService");

    hr = m_pDXVDService->GetDecoderDeviceGuids(&m_DeviceGuidCount, &m_DeviceGuidList);
    SMF_DX_CHECK(hr, "Fail to  get decoder device guids");
}

void DXVA2Device::shutdown(void)
{    
    // TODO:
    // make sure d3d resource is release before
    // release d3d device and unload d3d9. dll
    if(m_DeviceGuidList)
    {
        SMF_FFMPEG_LOG_INFO("Shutdown DXVA2 device");

        CoTaskMemFree(m_DeviceGuidList);
        m_DeviceGuidList = NULL;
        m_DeviceGuidCount = 0;
    }

    SMF_DX_SAFE_RELEASE(m_pDXVDService, "Release DirectXVideoDecoderService");

    if(m_pD3DDeviceManager && m_D3DDeviceHandle)
    {
        m_pD3DDeviceManager->CloseDeviceHandle(m_D3DDeviceHandle);

        m_D3DDeviceHandle = NULL;
    }
}



HWAcceleration* DXVA2Device::createHWAccel(AVCodecContext* pContext)
{
    GUID* pDeviceGuid = getDecoderGuid(pContext->codec_id);

    DXVA2HWAccel* pHWAccel = m_DXVA2HWAccelFactory.create(pContext,
                                                          m_pD3DDeviceManager, m_D3DDeviceHandle,
                                                          m_pDXVDService, *pDeviceGuid);

    pHWAccel->initialize();

    pContext->get_format =DXVA2Device:: getFormat;
    pContext->get_buffer2 = DXVA2Device::getBuffer;

    return pHWAccel;
}

void DXVA2Device::destroyHWAccel(struct AVCodecContext* pContext)
{
    assert(pContext);

    DXVA2HWAccel* pHWAccel = (DXVA2HWAccel*)pContext->opaque;
    assert(pHWAccel);

    m_DXVA2HWAccelFactory.destroy(pHWAccel);
}


//===============================================================================

enum AVPixelFormat DXVA2Device::getFormat(AVCodecContext* pContext , const enum AVPixelFormat *pix_fmts)
{
    try
    {
        DXVA2HWAccel* pHWAccel = (DXVA2HWAccel*)pContext->opaque;
        if(pHWAccel)
        {
            pHWAccel->initDecoder();
        }

        for (int i = 0; pix_fmts[i] != AV_PIX_FMT_NONE; i++)
        {
            if (pix_fmts[i] == AV_PIX_FMT_DXVA2_VLD)
                return AV_PIX_FMT_DXVA2_VLD;
        }

        return AV_PIX_FMT_NONE;
    }
    catch(std::exception& )
    {
        return AV_PIX_FMT_NONE;
    }
}

int DXVA2Device::getBuffer(AVCodecContext* pContext, AVFrame* pFrame, int flags)
{    
    DXVA2HWAccel* pHWAccel = (DXVA2HWAccel*)pContext->opaque;
    assert(pHWAccel);

    return pHWAccel->getBuffer(pFrame, flags);
}

GUID* DXVA2Device::getDecoderGuid(AVCodecID codec)
{
    if(codec == AV_CODEC_ID_H264)
    {
        for(int i=0; g_DeviceGUIDs[i]; i++)
        {
            const GUID& desireGuid =  *g_DeviceGUIDs[i];

            for (uint j = 0; j < m_DeviceGuidCount; j++)
            {
                if (IsEqualGUID(desireGuid, m_DeviceGuidList[j]))
                {
                    return &m_DeviceGuidList[j];
                }
            }
        }
    }

    SMF_FFMPEG_THROW(InternalErrorException, "Unsupport codec");
}

D3DFORMAT DXVA2Device::getRenderTargetFormat(const GUID& decoderGuid)
{
    UINT count = 0;
    D3DFORMAT* formatList = NULL;

    HRESULT hr  = m_pDXVDService->GetDecoderRenderTargets(decoderGuid, &count, &formatList);
    SMF_DX_CHECK(hr, "Fail to  get decoder RenderTargets");

    D3DFORMAT format = D3DFMT_UNKNOWN;
    for (uint j = 0; j < count; j++)
    {
        if (formatList[j] == MKTAG('N','V','1','2'))
        {
            format = formatList[j];
            break;
        }
    }

    CoTaskMemFree(formatList);
    return format;
}



} // namespace mf
} // namespace surveon


