#ifndef SMF_RTSP_VIDEO_STREAM_SOURCE_H
#define SMF_RTSP_VIDEO_STREAM_SOURCE_H

#include "SMFLIVE555Prerequisites.h"

namespace surveon
{
namespace mf
{

/**
 */
class RTSPVideoStreamSource : public IStreamSource
{
public:
    RTSPVideoStreamSource(MediaSubsession* subsession);
    virtual ~RTSPVideoStreamSource();

    virtual const MediaType& getMediaType(void) const;

    virtual void setActive(bool active);

    virtual bool isActive(void) const;

    virtual int getStreamIndex(void) const;

    virtual void getStatistics(Statistics& stat) const;

private:
    MediaSubsession* m_Subsession;

    MediaType m_MediaType;
};

} // namespace mf
} // namespace surveon

#endif // SMF_RTSP_VIDEO_STREAM_SOURCE_H


