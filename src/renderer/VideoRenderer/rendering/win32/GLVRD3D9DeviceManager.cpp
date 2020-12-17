#include "GLVRD3D9DeviceManager.h"
#include "GLVRWin32RenderWindow.h"
#include <glew/wglew.h>

namespace surveon
{
namespace glvr
{

D3D9DeviceManager::D3D9DeviceManager(IDirect3D9Ex* pD3D9, UINT adapter) :
    m_pD3D9(pD3D9),
    m_pD3DDevice(NULL),
    m_pDeviceManager(NULL),
    m_DeviceHandle(NULL),
    m_ResetToken(0),
    m_Adapter(adapter),
    m_hWnd(NULL)
{
    assert(m_pD3D9);

    m_hWnd = Win32RenderWindow::createWindow(100, 100, "D3DWindow");

    cretateD3DDevice();

    createDirect3DDeviceManager();

    assert(WGLEW_NV_DX_interop);    
}

D3D9DeviceManager::~D3D9DeviceManager()
{
    GLVR_DX_SAFE_RELEASE_LOG(m_pDeviceManager, "release Direct3D device manager");

    GLVR_DX_SAFE_RELEASE_LOG(m_pD3DDevice, "release Direct3D device");

    DestroyWindow(m_hWnd);
}

void* D3D9DeviceManager::getNativeDeviceHandle(void)
{
    return m_pDeviceManager;
}

uint D3D9DeviceManager::getAdaptorNumber(void) const
{
    return m_Adapter;
}

HardwarePixelBufferSharedPtr D3D9DeviceManager::createSurface(uint32 width, uint32 height)
{    
    HardwarePixelBufferSharedPtr pPixelBuffer;

    int count = 10;

    while(count)
    {
        try
        {
            D3D9Surface* pD3D9Surface = new D3D9Surface(m_pDeviceManager, m_DeviceHandle);

            pPixelBuffer.reset(pD3D9Surface);

            pD3D9Surface->create(width, height);

            return pPixelBuffer;
        }
        catch(GLVROpenGLErrorException& )
        {
            // work-around for
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            count--;
        }
    }

    return pPixelBuffer;
}

void D3D9DeviceManager::cretateD3DDevice(void)
{
    GLVR_LOG_INFO("create Direct3D device");

    D3DPRESENT_PARAMETERS d3dpp = {0};

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = m_hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

    HRESULT hr = m_pD3D9->CreateDeviceEx(m_Adapter, D3DDEVTYPE_HAL, m_hWnd,
                                 D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                 &d3dpp, NULL, &m_pD3DDevice);
    GLVR_DX_CHECK(hr, "Fail to create Direct3D device");
}

void D3D9DeviceManager::createDirect3DDeviceManager(void)
{
    GLVR_LOG_INFO("create Direct3D device manager");

    HRESULT hr = pfDXVA2CreateDirect3DDeviceManager9(&m_ResetToken, &m_pDeviceManager);
    GLVR_DX_CHECK(hr, "Failed to create Direct3D device manager");

    hr = m_pDeviceManager->ResetDevice(m_pD3DDevice, m_ResetToken);
    GLVR_DX_CHECK(hr, "Failed to bind Direct3D device to device manager");

    hr = m_pDeviceManager->OpenDeviceHandle(&m_DeviceHandle);
    GLVR_DX_CHECK(hr, "Failed to bind Direct3D device to device manager");
}


} // namespace glvr
} // namespace surveon


