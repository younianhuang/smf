#ifndef GLVR_D3D9_DEFINES_H
#define GLVR_D3D9_DEFINES_H

#include <stdint.h>
#include <d3d9.h>
#include <dxva2api.h>


#define GLVR_DX_SAFE_RELEASE(ptr) \
   if(ptr) { \
     ptr->Release(); \
     ptr = NULL; \
   }

#define GLVR_DX_SAFE_RELEASE_LOG(ptr, msg) \
   if(ptr) { \
     GLVR_LOG_INFO(msg); \
     ptr->Release(); \
     ptr = NULL; \
   }

#define GLVR_DX_CHECK(hr, msg) \
    if (FAILED(hr)) \
    {\
         GLVR_THROW_EXCEPT(InternalErrorException, msg <<" : " << hr );   \
    }

#define GLVR_DX_LOG_FAILED(hr, msg) \
    if (FAILED(hr)) \
    {\
         GLVR_LOG_ERROR(msg <<" : " << hr );   \
    }

namespace surveon
{
namespace glvr
{

typedef HRESULT WINAPI FnDirect3DCreate9Ex(UINT, IDirect3D9Ex **);

typedef HRESULT WINAPI  FnDXVA2CreateDirect3DDeviceManager9(UINT *, IDirect3DDeviceManager9 **);

extern FnDirect3DCreate9Ex*  pfDirect3DCreate9Ex;

extern FnDXVA2CreateDirect3DDeviceManager9* pfDXVA2CreateDirect3DDeviceManager9;

class D3DDeviceLock
{
public:
    D3DDeviceLock(IDirect3DDeviceManager9* pDeviceManager, HANDLE deviceHandle) :
        m_pDeviceManager(pDeviceManager),
        m_pDevice(NULL),
        m_DeviceHandle(deviceHandle)
    {
        assert(pDeviceManager);        
    }

    ~D3DDeviceLock(void)
    {
        unlock();        
    }

    IDirect3DDevice9* lock(void)
    {
        HRESULT hr = m_pDeviceManager->LockDevice(m_DeviceHandle, &m_pDevice, TRUE);
        GLVR_DX_CHECK(hr, "Fail to lock Direct3D9 device");

        return m_pDevice;
    }

    void unlock(void)
    {
        if(m_pDevice)
        {
            HRESULT hr =  m_pDeviceManager->UnlockDevice(m_DeviceHandle, FALSE);
            GLVR_DX_LOG_FAILED(hr, "Fail to unlock Direct3D9 device");

            m_pDevice = NULL;
        }
    }

private:
    IDirect3DDeviceManager9* m_pDeviceManager;

    IDirect3DDevice9* m_pDevice;

    HANDLE m_DeviceHandle;
};


} // namespace glvr
} // namespace surveon

#endif // GLVR_D3D9_DEFINES_H

