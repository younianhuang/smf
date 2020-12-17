#ifndef SMF_DXVA2_HELPER_H
#define SMF_DXVA2_HELPER_H

#include <windows.h>

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600

#include <stdint.h>
#include <d3d9.h>
#include <dxva2api.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "libavcodec/dxva2.h"

#include "libavutil/avassert.h"
#include "libavutil/buffer.h"
#include "libavutil/frame.h"
#include "libavutil/pixfmt.h"
}

#include "framework/SMFUtil.h"
#include "../SMFffmpegException.h"


extern "C" const GUID DXVADDI_Intel_ModeH264_E;

namespace surveon
{
namespace mf
{


typedef HRESULT WINAPI FnDirect3DCreate9Ex(UINT, IDirect3D9Ex **);

typedef HRESULT  WINAPI FnDXVA2CreateVideoService( IDirect3DDevice9* ,   REFIID ,   void** );


class DXVAHelper : public mf::Singleton<DXVAHelper>
{
    friend class Singleton<DXVAHelper>;

public:
    DXVAHelper(void);

    ~DXVAHelper(void);

    void initialize(void);

    void shutdown(void);

    IDirect3DDevice9Ex* cretateD3D9Device(void);

    IDirectXVideoDecoderService* createDirectXVideoDecoderService(IDirect3DDevice9* pD3DDevice);

private:
    void loadLibrary(const String& name, HMODULE& hModule);

    void unloadLibrary(const String& name, HMODULE& hModule);

    void* getSymbol(HMODULE& hModule, const String& funName);

    void createD3D9Object(void);

private:
    HMODULE m_LibD3D9;

    HMODULE m_LibDXVA2;

    IDirect3D9Ex* m_pD3D9;
};


} // namespace mf
} // namespace surveon


#define SMF_DX_SAFE_RELEASE(ptr, msg) \
   if(ptr) { \
     ptr->Release(); \
     SMF_FFMPEG_LOG_INFO(msg); \
     ptr = NULL; \
   }

#define SMF_DX_CHECK(hr, errMsg) \
    if (FAILED(hr)) \
    {\
         SMF_FFMPEG_THROW(InternalErrorException, errMsg <<" : " << hr );   \
    }


#endif // SMF_DXVA2_HELPER_H
