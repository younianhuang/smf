
#include "SMFRTSPVideoStreamSource.h"


namespace surveon
{
namespace mf
{

RTSPVideoStreamSource::RTSPVideoStreamSource(MediaSubsession* subsession) :
    m_Subsession(subsession)
{
    assert(subsession);
    m_MediaType.majoryType = SMF_MT_VIDEO;

    if(strcmp(m_Subsession->codecName(), "H264") == 0)
    {
        m_MediaType.subType = SMF_VIDEO_FORMAT_H264;
    }
}

RTSPVideoStreamSource::~RTSPVideoStreamSource()
{

}

const MediaType& RTSPVideoStreamSource::getMediaType(void) const
{
    return m_MediaType;
}

void RTSPVideoStreamSource::setActive(bool active)
{

}

bool RTSPVideoStreamSource::isActive(void) const
{
    return false;
}

int RTSPVideoStreamSource::getStreamIndex(void) const
{
    return 0;
}

void RTSPVideoStreamSource::getStatistics(Statistics& stat) const
{

}

} // namespace mf
} // namespace surveon


