#ifndef SMF_DXVA2_HWACCEL_H
#define SMF_DXVA2_HWACCEL_H


#include "../SMFffmpegException.h"
#include "../SMFHWAcceleration.h"
#include "SMFDXVA2Helper.h"

namespace surveon
{
namespace mf
{


class DXVA2HWAccel : public HWAcceleration
{
public:

    struct SurfaceInfo
    {
        bool used {false};
        uint64_t age {0};
    } ;

public:
    DXVA2HWAccel(AVCodecContext* pContext, IDirect3DDeviceManager9* pDeviceManager,
    HANDLE  deviceHandle,  IDirectXVideoDecoderService* pDXVDService,
                 const GUID&  decoderGUID);

    virtual ~DXVA2HWAccel(void);

    virtual void initialize(void);

    virtual void shutdown(void);

    virtual void retriveVideoData(AVFrame* pFrame, IVideoBuffer* pVideoBuffer);

    virtual void setDirectVideoOutTarget(mf::WindowHandle handle);

    virtual MediaSubType getFormat(void) const;

    int getBuffer(AVFrame* pFrame, int flags);

    void initDecoder(void);
    void shutdownDecoder(void);

public:

    static void releaseBuffer(void *opaque, uint8_t *data);

private:

    void getDecoderConfiguration(const DXVA2_VideoDesc& desc,  DXVA2_ConfigPictureDecode& config);


private:
    AVCodecContext* m_pAVContext;

    dxva_context* m_pDXVAContent;

    IDirect3DDeviceManager9* m_pD3DDeviceManager;

    HANDLE  m_D3DDeviceHandle;

    IDirectXVideoDecoderService* m_pDXVDService;

    GUID m_DecoderGUID;

    DXVA2_ConfigPictureDecode m_DecoderConfig;

    D3DFORMAT m_RenderTargetFormat;

    SurfaceInfo* m_SurfaceInfos;

    int m_SurfaceIndex;


};


} // namespace mf
} // namespace surveon


#endif // SMF_DXVA2_HWACCEL_H



