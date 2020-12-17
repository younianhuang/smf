
#ifndef GLVR_DX9_SURFACE_H
#define GLVR_DX9_SURFACE_H


#include "GLVRWin32Prerequisites.h"
#include "../GLVRPixelBuffer.h"
#include "GLVRD3D9Defines.h"

namespace surveon
{
namespace glvr
{

class D3D9Surface : public HardwarePixelBuffer
{
public:    
    D3D9Surface(IDirect3DDeviceManager9* pDeviceManager, HANDLE deviceHandle);

    virtual ~D3D9Surface();

    virtual void* getNativeHandle(void);

    virtual void* getPrivateData(void);

    virtual void bindToTexture(uint texureId);

    virtual void unbindToTexture(void);

    virtual uint32 getWidth(void) const;

    virtual uint32 getHeight(void) const;

    virtual mf::MediaSubType getFormat(void) const;

    virtual void blit(HardwarePixelBufferSharedPtr pSource);

    void create(uint width, uint height);

    void destroy(void);

private:
    IDirect3DDevice9* m_pD3DDevice;
    IDirect3DDeviceManager9* m_pDeviceManager;
    IDirect3DSurface9* m_pSurface;
    HANDLE m_DeviceHandle;
    HANDLE m_GLXDeviceHandle;
    HANDLE m_GLXShareSurfaceHandle;
    HANDLE m_GLXShareTextureHandle;
    uint m_Width;
    uint m_Height;
    bool m_BindingToTexture;
};



} // namespace glvr
} // namespace surveon

#endif // GLVR_DX9_SURFACE_H



