#include "GLVRD3D9Surface.h"
#include <glew/wglew.h>

namespace surveon
{
namespace glvr
{

D3D9Surface::D3D9Surface(IDirect3DDeviceManager9* pDeviceManager, HANDLE deviceHandle) :
    m_DeviceHandle(deviceHandle),
    m_pDeviceManager(pDeviceManager),
    m_pSurface(NULL),    
    m_GLXShareSurfaceHandle(NULL),
    m_GLXShareTextureHandle(NULL),
    m_Width(0),
    m_Height(0),
    m_BindingToTexture(false)
{
    assert(pDeviceManager);
}

D3D9Surface::~D3D9Surface()
{
    destroy();
}

void* D3D9Surface::getNativeHandle(void)
{
    return m_pSurface;
}

void* D3D9Surface::getPrivateData(void)
{
    return m_pSurface;
}

void D3D9Surface::bindToTexture(uint texureId)
{
    if(!m_GLXShareTextureHandle)
    {
        m_GLXShareTextureHandle = wglDXRegisterObjectNV(m_GLXDeviceHandle,
                                                               m_pSurface,
                                                               texureId,
                                                               GL_TEXTURE_2D,
                                                               WGL_ACCESS_READ_ONLY_NV);
        if(m_GLXShareTextureHandle == 0)
        {
            GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to register Direct3D9 surface to OpenGL texture  : "
                              << GetLastError());
        }
    }

    if(wglDXLockObjectsNV(m_GLXDeviceHandle, 1, &m_GLXShareTextureHandle) == FALSE)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to lock share texture handle : "
                          << GetLastError() );
    }

    m_BindingToTexture = true;
}

void D3D9Surface::unbindToTexture(void)
{
    if (m_BindingToTexture)
    {
        if(wglDXUnlockObjectsNV(m_GLXDeviceHandle, 1, &m_GLXShareTextureHandle) == FALSE)
        {
            GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to unlock share texture handle : "
                              << GetLastError());
        }

        m_BindingToTexture = false;
    }
}

uint32 D3D9Surface::getWidth(void) const
{
    return m_Width;
}

uint32 D3D9Surface::getHeight(void) const
{
    return m_Height;
}

mf::MediaSubType D3D9Surface::getFormat(void) const
{
    return SMF_VIDEO_FORMAT_D3D9_SURFACE;
}

void D3D9Surface::blit(HardwarePixelBufferSharedPtr pSource)
{
    D3DDeviceLock lockGuard(m_pDeviceManager, m_DeviceHandle);

    IDirect3DDevice9* pDevice = lockGuard.lock();

    D3D9Surface*pSourceSurface = dynamic_cast<D3D9Surface*>(pSource.get());
    assert(pSourceSurface);

    HRESULT hr = pDevice->StretchRect(pSourceSurface->m_pSurface, NULL, m_pSurface, NULL, D3DTEXF_NONE);
    GLVR_DX_CHECK(hr, "Fail to copy Direct3D9 surface");
}

//=========================================================================

void D3D9Surface::create(uint width, uint height)
{
    m_Width = width;
    m_Height = height;

    assert(m_Width);
    assert(m_Height);

    D3DDeviceLock lockGuard(m_pDeviceManager, m_DeviceHandle);

    IDirect3DDevice9* pDevice = lockGuard.lock();

    HRESULT hr = pDevice->CreateRenderTarget(
                m_Width,
                m_Height,
                D3DFMT_X8R8G8B8,
                D3DMULTISAMPLE_NONE,
                0,
                FALSE,
                &m_pSurface,
                &m_GLXShareSurfaceHandle);
    GLVR_DX_CHECK(hr, "Fail to create Direct3D9 surface");

    m_GLXDeviceHandle = wglDXOpenDeviceNV(pDevice);
    if(!m_GLXDeviceHandle)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to open GLX device handle : " <<  GetLastError());
    }

    if(wglDXSetResourceShareHandleNV(m_pSurface, m_GLXShareSurfaceHandle) == FALSE)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to share D3D9 surface to GLX : " <<  GetLastError());
    }
}

void D3D9Surface::destroy(void)
{   
    unbindToTexture();

    if(m_GLXShareTextureHandle)
    {
        if(wglDXUnregisterObjectNV(m_GLXDeviceHandle, m_GLXShareTextureHandle) == FALSE)
        {
            GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to unregister share texture handle : "
                              << GetLastError());
        }

        m_GLXShareTextureHandle = 0;
    }

    if(m_GLXDeviceHandle)
    {
        if(!wglDXCloseDeviceNV(m_GLXDeviceHandle))
        {
            GLVR_LOG_ERROR("Fail to close GLX device handle");
        }

        m_GLXDeviceHandle = 0;
    }

    GLVR_DX_SAFE_RELEASE(m_pSurface);
}



} // namespace glvr
} // namespace surveon


