#ifndef DX9DEVICEMANAGER_H
#define DX9DEVICEMANAGER_H

#include "GLVRWin32Prerequisites.h"
#include "GLVRD3D9Defines.h"
#include "GLVRD3D9Surface.h"
#include <framework/pipeline/SMFIDeviceManager.h>

namespace surveon
{
namespace glvr
{

class D3D9DeviceManager: public mf::IDeviceManager
{
public:
    D3D9DeviceManager(IDirect3D9Ex* pD3D9, UINT adapter);

    virtual ~D3D9DeviceManager();

    virtual void* getNativeDeviceHandle(void);

    virtual uint getAdaptorNumber(void) const;

    HardwarePixelBufferSharedPtr createSurface(uint32 width, uint32 height);

private:
    void cretateD3DDevice(void);

    void createDirect3DDeviceManager(void);

private:
    IDirect3D9Ex* m_pD3D9;

    IDirect3DDevice9Ex* m_pD3DDevice;

    IDirect3DDeviceManager9* m_pDeviceManager;

    HANDLE m_DeviceHandle;

    UINT m_ResetToken;

    UINT m_Adapter;

    HWND m_hWnd;
};

} // namespace glvr
} // namespace surveon

#endif // DX9DEVICEMANAGER_H
