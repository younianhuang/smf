#ifndef GLVR_D3D9_MANAGER_H
#define GLVR_D3D9_MANAGER_H

#include "GLVRWin32Prerequisites.h"
#include "GLVRD3D9Defines.h"
#include "GLVRD3D9DeviceManager.h"

namespace surveon
{
namespace glvr
{

class D3D9Manager: public mf::Singleton<D3D9Manager>
{
    friend class Singleton<D3D9Manager>;

public:
    void initialize(void);

    void shutdown(void);

    D3D9DeviceManager* getD3D9DeviceManager(void);

private:
    D3D9Manager();

    ~D3D9Manager();

    void loadLibrary(const String& name, HMODULE& hModule);

    void unloadLibrary(const String& name, HMODULE& hModule);

    void* getSymbol(HMODULE& hModule, const String& funName);

    void createD3D9Object(void);

    void createD3D9DeviceManager(void);

private:
    HMODULE m_LibD3D9;

    HMODULE m_LibDXVA2;

    IDirect3D9Ex* m_pD3D9;

    std::unique_ptr<D3D9DeviceManager> m_pD3D9DeviceManager;
};


} // namespace glvr
} // namespace surveon

#endif // D3D9Manager_H


