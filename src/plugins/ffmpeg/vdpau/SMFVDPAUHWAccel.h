#ifndef SMF_VDPAU_HWACCEL_H
#define SMF_VDPAU_HWACCEL_H


#include "../SMFffmpegException.h"
#include "../SMFHWAcceleration.h"
#include "SMFVDPAUHelper.h"


namespace surveon
{
namespace mf
{


class VDPAUHWAccel : public HWAcceleration
{
public:
    VDPAUHWAccel(AVCodecContext* pContext, VdpDevice device);

    virtual ~VDPAUHWAccel(void);

    virtual void initialize(void);

    virtual void shutdown(void);

    virtual void retriveVideoData(AVFrame* pFrame, IVideoBuffer* pVideoBuffer);

    virtual void setDirectVideoOutTarget(mf::WindowHandle handle);

    virtual MediaSubType getFormat(void) const;

    void open(void);
    void close(void);

    int getBuffer(AVFrame* pFrame, int flags);

    //void drawHorizBand(const AVFrame* pSrcFrame);

    //void retriveVideoData(AVFrame* pFrame, IMediaBuffer* pMediaBuffer);

    //void setVideoOutTarget(Drawable drawable);

public:

    static void releaseBuffer(void *opaque, uint8_t *data);

private:
    void getSurfaceParameters(void);

    //void createVdpDecoder(void);

    //void destroyVdpDecoder(void);

    void createVdpVideoMixer(void);

    void destroyVdpVideoMixer(void);

    void createVdpPresentationQueue(void);

    void destroyVdpPresentationQueue(void);

    vdpau_render_state* getFreeRenderState(void);

    void destroyAllRenderStates(void);

private:
    AVCodecContext* m_pAVCodecContext;

    VdpDevice m_VdpDevice;
    VdpDecoder m_VdpDecoder;
    VdpVideoMixer m_VdpVideoMixer;
    VdpOutputSurface m_OutputSurface;

    VdpPresentationQueue m_VdpPresentationQueue;
    VdpPresentationQueueTarget m_VdpPresentationQueueTarget;
    Drawable m_Drawable;

    uint32_t m_InputWidth;
    uint32_t m_InputHeight;
    VdpChromaType m_InputChroma;

    std::vector<vdpau_render_state*> m_RenderStates;
};


} // namespace mf
} // namespace surveon


#endif // SMF_VDPAU_HWACCEL_H
