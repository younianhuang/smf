#ifndef SMF_VDPAU_HELPER_H
#define SMF_VDPAU_HELPER_H


#include "../SMFffmpegPrerequisites.h"
#include "../SMFffmpegLog.h"
#include "../SMFffmpegException.h"


extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "libavcodec/vdpau.h"

#include "libavutil/avassert.h"
#include "libavutil/buffer.h"
#include "libavutil/frame.h"
#include "libavutil/pixfmt.h"
}


namespace surveon
{
namespace mf
{


extern VdpGetProcAddress* vdp_get_proc_address;

extern VdpVideoSurfaceGetParameters* vdp_video_surface_get_parameters;
extern VdpVideoSurfaceGetBitsYCbCr* vdp_video_surface_get_bits_y_cb_cr;
extern VdpVideoSurfaceCreate* vdp_video_surface_create;
extern VdpVideoSurfaceDestroy* vdp_video_surface_destroy;

extern VdpDeviceDestroy* vdp_device_destroy;

extern VdpDecoderCreate* vdp_decoder_create;
extern VdpDecoderDestroy* vdp_decoder_destroy;
extern VdpDecoderRender* vdp_decoder_render;

extern VdpOutputSurfaceCreate* vdp_output_surface_create;
extern VdpOutputSurfaceDestroy* vdp_output_surface_destroy;
extern VdpOutputSurfaceGetBitsNative* vdp_output_surface_get_bits_native;
extern VdpOutputSurfaceGetParameters* vdp_output_surface_get_parameters;
extern VdpOutputSurfacePutBitsNative* vdp_output_surface_put_bits_native;

extern VdpVideoMixerCreate* vdp_video_mixer_create;
extern VdpVideoMixerDestroy* vdp_video_mixer_destroy;
extern VdpVideoMixerRender* vdp_video_mixer_render;

extern VdpPresentationQueueCreate* vdp_presentation_queue_create;
extern VdpPresentationQueueDestroy* vdp_presentation_queue_destroy;
extern VdpPresentationQueueGetTime* vdp_presentation_queue_get_time;
extern VdpPresentationQueueTargetCreateX11* vdp_presentation_queue_target_create_x11;
extern VdpPresentationQueueTargetDestroy* vdp_presentation_queue_target_destroy;
extern VdpPresentationQueueQuerySurfaceStatus*
        vdp_presentation_queue_query_surface_status;
extern VdpPresentationQueueDisplay* vdp_presentation_queue_display;
extern VdpPresentationQueueBlockUntilSurfaceIdle*
        vdp_presentation_queue_block_until_surface_idle;

extern VdpGetApiVersion* vdp_get_api_version;
extern VdpGetInformationString* vdp_get_information_string;
extern VdpGetErrorString* vdp_get_error_string;


class VDPAUHelper
{

public:

    static void getProcAddress(VdpDevice device, VdpFuncId functionId, void** functionPointer);


};



#define SMF_VDPAU_CHECK(vdpaufun)   \
        { \
            SMF_FFMPEG_LOG_TRACE(#vdpaufun); \
            VdpStatus status = vdpaufun;  \
            if(status != VDP_STATUS_OK) \
            {   \
                SMF_FFMPEG_THROW(InternalErrorException, \
                "Fail to call " << #vdpaufun << " : " << vdp_get_error_string(status)); \
            }  \
        }

#define SMF_VDPAU_TRACE(vdpaufun)   \
        { \
            SMF_FFMPEG_LOG_TRACE(#vdpaufun); \
            VdpStatus status = vdpaufun;  \
            if(status != VDP_STATUS_OK) \
            {   \
                SMF_FFMPEG_LOG_WARN("Fail to call " \
                   << #vdpaufun << " : " << vdp_get_error_string(status)); \
            }  \
        }


} // namespace mf
} // namespace surveon



#endif // SMF_VDPAU_HELPER_H
