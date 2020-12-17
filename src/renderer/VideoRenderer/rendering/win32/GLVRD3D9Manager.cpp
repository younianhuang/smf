#include "GLVRD3D9Manager.h"


namespace surveon
{
namespace glvr
{

FnDirect3DCreate9Ex*  pfDirect3DCreate9Ex = nullptr;

FnDXVA2CreateDirect3DDeviceManager9* pfDXVA2CreateDirect3DDeviceManager9 = nullptr;

D3D9Manager::D3D9Manager() :
    m_LibD3D9(NULL),
    m_LibDXVA2(NULL),
    m_pD3D9(NULL)
{
    initialize();
}

D3D9Manager::~D3D9Manager(void)
{
    shutdown();
}

void D3D9Manager::initialize(void)
{
    loadLibrary("d3d9.dll", m_LibD3D9);

    loadLibrary("dxva2.dll", m_LibDXVA2);

    pfDirect3DCreate9Ex = (FnDirect3DCreate9Ex*)getSymbol(m_LibD3D9, "Direct3DCreate9Ex");
    assert(pfDirect3DCreate9Ex);

    pfDXVA2CreateDirect3DDeviceManager9 = (FnDXVA2CreateDirect3DDeviceManager9*)getSymbol(m_LibDXVA2, "DXVA2CreateDirect3DDeviceManager9");
    assert(pfDXVA2CreateDirect3DDeviceManager9);

    createD3D9Object();

    createD3D9DeviceManager();
}

void D3D9Manager::shutdown(void)
{
    m_pD3D9DeviceManager.reset();

    GLVR_DX_SAFE_RELEASE_LOG(m_pD3D9, "Release Direct3D9");

    unloadLibrary("d3d9.dll", m_LibD3D9);

    unloadLibrary("dxva2.dll", m_LibDXVA2);
}

D3D9DeviceManager* D3D9Manager::getD3D9DeviceManager(void)
{
    return m_pD3D9DeviceManager.get();
}


//===================================================================

void D3D9Manager::loadLibrary(const String& name, HMODULE& hModule)
{
    GLVR_LOG_INFO("Load library " << name);

    hModule =  LoadLibraryA(name.c_str());
    if(!hModule)
    {
        GLVR_THROW_EXCEPT(GLVRInternalErrorException, "Fail to load library " << name);
    }
}

void D3D9Manager::unloadLibrary(const String& name, HMODULE& hModule)
{
    if(hModule)
    {
        GLVR_LOG_INFO("Unload library " << name);

        if(!FreeLibrary( hModule ) )
        {
            GLVR_LOG_ERROR("Fail to unload library " << name);
        }

        hModule = NULL;
    }
}

void* D3D9Manager::getSymbol( HMODULE& hModule, const String& funName )
{
    void* pf = GetProcAddress(hModule, funName.c_str());
    if(!pf)
    {
        GLVR_THROW_EXCEPT(GLVRInternalErrorException, "Fail to get Symbol " << funName);
    }

    return pf;
}

void D3D9Manager::createD3D9Object(void)
{
    GLVR_LOG_INFO("create Direct3D9");

    HRESULT hr = pfDirect3DCreate9Ex(D3D_SDK_VERSION, &m_pD3D9);
    GLVR_DX_CHECK(hr, "Fail to create Direct3D9Ex");

    hr = m_pD3D9->CheckDeviceFormatConversion(0, D3DDEVTYPE_HAL,
                                              (D3DFORMAT) MAKEFOURCC('N','V','1','2'), D3DFMT_X8R8G8B8);
    GLVR_DX_CHECK(hr, "Direct3D9 does not support NV12 to X8R8G8B8 color conversion");
}

void D3D9Manager::createD3D9DeviceManager(void)
{
    m_pD3D9DeviceManager.reset(new D3D9DeviceManager(m_pD3D9, D3DADAPTER_DEFAULT));
}


} // namespace glvr
} // namespace surveon



