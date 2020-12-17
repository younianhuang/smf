
#include "SMFVDPAUHelper.h"


namespace surveon
{
namespace mf
{


VdpGetProcAddress* vdp_get_proc_address;

VdpVideoSurfaceGetParameters* vdp_video_surface_get_parameters;
VdpVideoSurfaceGetBitsYCbCr* vdp_video_surface_get_bits_y_cb_cr;
VdpVideoSurfaceCreate* vdp_video_surface_create;
VdpVideoSurfaceDestroy* vdp_video_surface_destroy;

VdpDeviceDestroy* vdp_device_destroy;

VdpDecoderCreate* vdp_decoder_create;
VdpDecoderDestroy* vdp_decoder_destroy;
VdpDecoderRender* vdp_decoder_render;

VdpOutputSurfaceCreate* vdp_output_surface_create;
VdpOutputSurfaceDestroy* vdp_output_surface_destroy;
VdpOutputSurfaceGetBitsNative* vdp_output_surface_get_bits_native;
VdpOutputSurfaceGetParameters* vdp_output_surface_get_parameters;
VdpOutputSurfacePutBitsNative* vdp_output_surface_put_bits_native;


VdpVideoMixerCreate* vdp_video_mixer_create;
VdpVideoMixerDestroy* vdp_video_mixer_destroy;
VdpVideoMixerRender* vdp_video_mixer_render;

VdpPresentationQueueCreate* vdp_presentation_queue_create;
VdpPresentationQueueDestroy* vdp_presentation_queue_destroy;
VdpPresentationQueueGetTime* vdp_presentation_queue_get_time;
VdpPresentationQueueTargetCreateX11* vdp_presentation_queue_target_create_x11;
VdpPresentationQueueQuerySurfaceStatus* vdp_presentation_queue_query_surface_status;
VdpPresentationQueueTargetDestroy* vdp_presentation_queue_target_destroy;
VdpPresentationQueueDisplay* vdp_presentation_queue_display;
VdpPresentationQueueBlockUntilSurfaceIdle* vdp_presentation_queue_block_until_surface_idle;

VdpGetApiVersion* vdp_get_api_version;
VdpGetInformationString* vdp_get_information_string;
VdpGetErrorString* vdp_get_error_string;



void VDPAUHelper::getProcAddress(VdpDevice device, VdpFuncId functionId, void** functionPointer)
{
    VdpStatus status = vdp_get_proc_address(device, functionId, functionPointer);
    if(status != VDP_STATUS_OK)
    {
        SMF_FFMPEG_THROW(InternalErrorException, "fail to get address of proc:" <<  functionId);
    }
}




} // namespace mf
} // namespace surveon
