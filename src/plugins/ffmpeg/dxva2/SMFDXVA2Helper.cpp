
#include "SMFDXVA2Helper.h"
#include <initguid.h>

DEFINE_GUID(IID_IDirectXVideoDecoderService, 0xfc51a551,0xd5e7,0x11d9,0xaf,0x55,0x00,0x05,0x4e,0x43,0xff,0x02);
DEFINE_GUID(DXVA2_ModeMPEG2_VLD,      0xee27417f, 0x5e28,0x4e65,0xbe,0xea,0x1d,0x26,0xb5,0x08,0xad,0xc9);
DEFINE_GUID(DXVA2_ModeMPEG2and1_VLD,  0x86695f12, 0x340e,0x4f04,0x9f,0xd3,0x92,0x53,0xdd,0x32,0x74,0x60);
DEFINE_GUID(DXVA2_ModeH264_E,         0x1b81be68, 0xa0c7,0x11d3,0xb9,0x84,0x00,0xc0,0x4f,0x2e,0x73,0xc5);
DEFINE_GUID(DXVA2_ModeH264_F,         0x1b81be69, 0xa0c7,0x11d3,0xb9,0x84,0x00,0xc0,0x4f,0x2e,0x73,0xc5);
DEFINE_GUID(DXVADDI_Intel_ModeH264_E, 0x604F8E68, 0x4951,0x4C54,0x88,0xFE,0xAB,0xD2,0x5C,0x15,0xB3,0xD6);
DEFINE_GUID(DXVA2_ModeVC1_D,          0x1b81beA3, 0xa0c7,0x11d3,0xb9,0x84,0x00,0xc0,0x4f,0x2e,0x73,0xc5);
DEFINE_GUID(DXVA2_ModeVC1_D2010,      0x1b81beA4, 0xa0c7,0x11d3,0xb9,0x84,0x00,0xc0,0x4f,0x2e,0x73,0xc5);
DEFINE_GUID(DXVA2_ModeHEVC_VLD_Main,  0x5b11d51b, 0x2f4c,0x4452,0xbc,0xc3,0x09,0xf2,0xa1,0x16,0x0c,0xc0);
DEFINE_GUID(DXVA2_NoEncrypt,          0x1b81beD0, 0xa0c7,0x11d3,0xb9,0x84,0x00,0xc0,0x4f,0x2e,0x73,0xc5);
DEFINE_GUID(GUID_NULL,                0x00000000, 0x0000,0x0000,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);

namespace surveon
{
namespace mf
{

//static FnDirect3DCreate9*  pfDirect3DCreate9 = nullptr;

static FnDirect3DCreate9Ex*  pfDirect3DCreate9Ex = nullptr;

static FnDXVA2CreateVideoService* pfDXVA2CreateVideoService = nullptr;

 DXVAHelper::DXVAHelper(void) :
    m_LibD3D9(NULL),
    m_LibDXVA2(NULL),
    m_pD3D9(NULL)
 {

 }


DXVAHelper::~DXVAHelper(void)
{
    shutdown();
}

void DXVAHelper::createD3D9Object(void)
{
    SMF_FFMPEG_LOG_INFO("create D3D9 ");

    HRESULT hr = pfDirect3DCreate9Ex(D3D_SDK_VERSION, &m_pD3D9);
    SMF_DX_CHECK(hr, "Fail to create D3D9Ex");
/*
    m_pD3D9 = pfDirect3DCreate9(D3D_SDK_VERSION);
    if(!m_pD3D9)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "Fail to create D3D9");
    }
*/
}

IDirectXVideoDecoderService* DXVAHelper::createDirectXVideoDecoderService(IDirect3DDevice9* pD3DDevice)
{
    SMF_FFMPEG_LOG_INFO("create DirectXVideoDecoderService ");

    IDirectXVideoDecoderService* pDXVDService = NULL;

    HRESULT hr = pfDXVA2CreateVideoService(pD3DDevice,
                                           IID_IDirectXVideoDecoderService,
                                           (void **)&pDXVDService);
     SMF_DX_CHECK(hr, "fail to create dxva2 video service");

     return pDXVDService;
}

void DXVAHelper::initialize(void)
{
    loadLibrary("d3d9.dll", m_LibD3D9);

    loadLibrary("dxva2.dll", m_LibDXVA2);

    pfDirect3DCreate9Ex = (FnDirect3DCreate9Ex*)getSymbol(m_LibD3D9, "Direct3DCreate9Ex");
    assert(pfDirect3DCreate9Ex);

    pfDXVA2CreateVideoService = (FnDXVA2CreateVideoService*)getSymbol(m_LibDXVA2, "DXVA2CreateVideoService");
    assert(pfDXVA2CreateVideoService);

    createD3D9Object();        
}

void DXVAHelper::shutdown(void)
{
    SMF_DX_SAFE_RELEASE(m_pD3D9, "Release D3D9");

    unloadLibrary("d3d9.dll", m_LibD3D9);

    unloadLibrary("dxva2.dll", m_LibDXVA2);
}

IDirect3DDevice9Ex* DXVAHelper::cretateD3D9Device(void)
{
    SMF_FFMPEG_LOG_INFO("create D3D9 device");

    UINT adapter = D3DADAPTER_DEFAULT;
    D3DDISPLAYMODE d3ddm;

    HRESULT hr = m_pD3D9->GetAdapterDisplayMode(adapter, &d3ddm);
    SMF_DX_CHECK(hr, "Fail to get adapter display mode");

    D3DPRESENT_PARAMETERS d3dpp = {0};

    d3dpp.Windowed         = TRUE;
    d3dpp.BackBufferWidth  = 320;
    d3dpp.BackBufferHeight = 240;
    d3dpp.BackBufferCount  = 0;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
    d3dpp.Flags            = D3DPRESENTFLAG_VIDEO;

    IDirect3DDevice9Ex* pD3DDevice = NULL;
    hr = m_pD3D9->CreateDeviceEx(adapter, D3DDEVTYPE_HAL, NULL,
                                 D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
                                 &d3dpp, NULL, &pD3DDevice);
    SMF_DX_CHECK(hr, "Fail to create d3d device");

    return pD3DDevice;
}



//===================================================================

void DXVAHelper::loadLibrary(const String& name, HMODULE& hModule)
{
    SMF_FFMPEG_LOG_INFO("Load library " << name);

    hModule =  LoadLibraryA(name.c_str());
    if(!hModule)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "Fail to load library " << name);
    }
}

void DXVAHelper::unloadLibrary(const String& name, HMODULE& hModule)
{
    if(hModule)
    {
        SMF_FFMPEG_LOG_INFO("Unload library " << name);

        if(!FreeLibrary( hModule ) )
        {
            SMF_FFMPEG_LOG_ERROR("Fail to unload library " << name);
        }

        hModule = NULL;
    }
}

void* DXVAHelper::getSymbol( HMODULE& hModule, const String& funName )
{
    void* pf = GetProcAddress(hModule, funName.c_str());
    if(!pf)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "Fail to get Symbol " << funName);
    }

    return pf;
}

} // namespace mf
} // namespace surveon




