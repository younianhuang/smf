#ifndef SMF_VAAPI_HWACCEL_H
#define SMF_VAAPI_HWACCEL_H


#include "../SMFffmpegException.h"
#include "../SMFHWAcceleration.h"
#include "SMFVAAPIHelper.h"

namespace surveon
{
namespace mf
{

const int VA_NUM_OF_RENDER_TARGETS = 6;


class VAAPIHWAccel : public HWAcceleration
{
public:
    struct RenderTargetState
    {
        VASurfaceID vaSurface {VA_INVALID_SURFACE};
        bool used {false};
    };

public:
    VAAPIHWAccel(AVCodecContext* pContext, Display* pDisplay, VADisplay vaDisplay);

    virtual ~VAAPIHWAccel(void);

    virtual void initialize(void);

    virtual void shutdown(void);

    virtual void retriveVideoData(AVFrame* pFrame, IVideoBuffer* pVideoBuffer);

    virtual void setDirectVideoOutTarget(mf::WindowHandle handle);

    virtual MediaSubType getFormat(void) const;

    int getBuffer(AVFrame* pFrame, int flags);

    RenderTargetState* getFreeRenderTarget(void);

    void initDecoder(void);
    void shutdownDecoder(void);

public:

    static void releaseBuffer(void *opaque, uint8_t *data);

private:


    int chooseRTFormat(int supportFormat);

private:
    AVCodecContext* m_pAVContext;
    vaapi_context* m_pVAContext;
    Display* m_pXDisplay;
    VADisplay m_VADisplay;    
    Drawable m_Drawable;
    uint16 m_WinWidth;
    uint16 m_WinHeight;

    VASurfaceID m_VASurfaces[VA_NUM_OF_RENDER_TARGETS];
    RenderTargetState m_RenderTargetStates[VA_NUM_OF_RENDER_TARGETS];

    int m_SurfaceIndex;
};


} // namespace mf
} // namespace surveon


#endif // SMF_VAAPI_HWACCEL_H



