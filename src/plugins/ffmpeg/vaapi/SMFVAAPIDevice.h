
#ifndef SMF_VAAPI_DEVICE_H
#define SMF_VAAPI_DEVICE_H


#include "../SMFffmpegPrerequisites.h"
#include "../SMFffmpegLog.h"
#include "../SMFffmpegException.h"
#include "../SMFHWAccelerationDevice.h"
#include "SMFVAAPIHWAccel.h"


namespace surveon
{
namespace mf
{


class VAAPIDevice : public HWAccelerationDevice
{

public:
    VAAPIDevice(void);

    virtual ~VAAPIDevice(void);

    virtual void initialize(void);

    virtual void shutdown(void);

    virtual HWAcceleration* createHWAccel(AVCodecContext* pContext);

    virtual void destroyHWAccel(struct AVCodecContext* pContext);

private:

    static enum AVPixelFormat getFormat(AVCodecContext *pContext,
                                        const enum AVPixelFormat *pix_fmts);

    static int getBuffer(AVCodecContext* pContext, AVFrame* pFrame, int flags);


private:
    Display* m_pXDisplay;
    VADisplay m_VADisplay;

    typedef SimpleObjectFactory<VAAPIHWAccel, Mutex> VDPAUDecoderFactory;
    VDPAUDecoderFactory m_VDPAUDecoderFactory;
};




} // namespace mf
} // namespace surveon



#endif // SMF_VAAPI_DEVICE_H

