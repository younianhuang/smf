#ifndef SMF_VDPAU_DEVICE_H
#define SMF_VDPAU_DEVICE_H


#include "../SMFffmpegPrerequisites.h"
#include "../SMFffmpegLog.h"
#include "../SMFffmpegException.h"
#include "../SMFHWAccelerationDevice.h"
#include "SMFVDPAUHWAccel.h"


namespace surveon
{
namespace mf
{


class VDPAUDevice : public HWAccelerationDevice
{

public:
    VDPAUDevice(void);

    virtual ~VDPAUDevice(void);

    virtual void initialize(void);

    virtual void shutdown(void);

    virtual HWAcceleration* createHWAccel(AVCodecContext* pContext);

    virtual void destroyHWAccel(struct AVCodecContext* pContext);

private:
    void getProcAddress(VdpFuncId functionId, void** functionPointer);

    static enum AVPixelFormat getFormat(AVCodecContext *pContext,
                                        const enum AVPixelFormat *pix_fmts);

    static int getBuffer(AVCodecContext* pContext, AVFrame* pFrame, int flags);

    //static void drawHorizBand(struct AVCodecContext* pContext, const AVFrame* pSrcFrame,
    //        int offset[4], int y, int type, int height);

private:
    Display* m_pXDisplay;
    VdpDevice m_VdpDevice;

    typedef SimpleObjectFactory<VDPAUHWAccel, Mutex> VDPAUDecoderFactory;
    VDPAUDecoderFactory m_VDPAUDecoderFactory;
};




} // namespace mf
} // namespace surveon



#endif // SMF_VDPAU_DEVICE_H
