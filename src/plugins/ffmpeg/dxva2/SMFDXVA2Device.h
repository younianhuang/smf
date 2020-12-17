
#ifndef SMF_DXVA2_DEVICE_H
#define SMF_DXVA2_DEVICE_H


#include "../SMFffmpegPrerequisites.h"
#include "../SMFffmpegLog.h"
#include "../SMFffmpegException.h"
#include "../SMFHWAccelerationDevice.h"
#include "SMFDXVA2HWAccel.h"


namespace surveon
{
namespace mf
{

class DXVA2Device : public HWAccelerationDevice
{

public:
    DXVA2Device(IDeviceManager* pDeviceManager);

    virtual ~DXVA2Device(void);

    virtual void initialize(void);

    virtual void shutdown(void);

    virtual HWAcceleration* createHWAccel(AVCodecContext* pContext);

    virtual void destroyHWAccel(struct AVCodecContext* pContext);

private:

    static enum AVPixelFormat getFormat(AVCodecContext *pContext,
                                        const enum AVPixelFormat *pix_fmts);

    static int getBuffer(AVCodecContext* pContext, AVFrame* pFrame, int flags);

    GUID* getDecoderGuid(AVCodecID codec);

    D3DFORMAT getRenderTargetFormat(const GUID& decoderGuid);

    void getDecoderConfiguration(const GUID& decoderGuid, DXVA2_ConfigPictureDecode& config);

private:

    typedef SimpleObjectFactory< DXVA2HWAccel, Mutex> DXVA2HWAccelFactory;
    DXVA2HWAccelFactory m_DXVA2HWAccelFactory;

    IDirect3DDeviceManager9* m_pD3DDeviceManager;

    IDirectXVideoDecoderService* m_pDXVDService;

    HANDLE  m_D3DDeviceHandle;

    GUID* m_DeviceGuidList;
    UINT m_DeviceGuidCount;
};


} // namespace mf
} // namespace surveon



#endif // SMF_DXVA2_DEVICE_H

