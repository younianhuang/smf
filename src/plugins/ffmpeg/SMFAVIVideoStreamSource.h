#ifndef SMF_AVI_VIDEO_STREAM_SOURCE_H
#define SMF_AVI_VIDEO_STREAM_SOURCE_H

#include "SMFffmpegException.h"
#include "SMFFFmpegHelper.h"


struct AVFormatContext;

namespace surveon
{
namespace mf
{

/**
 */
class AVIVideoStreamSource : public IStreamSource
{
public:
    AVIVideoStreamSource(int index, AVCodecContext* pCodecContext);
    virtual ~AVIVideoStreamSource();

    virtual const MediaType& getMediaType(void) const;

    virtual void setActive(bool active);

    virtual bool isActive(void) const;

    virtual int getStreamIndex(void) const;

    virtual void getStatistics(Statistics& stat) const;

    void resetStatistics(void);
    void updateStatistics(uint64 dataSizeInBits);

private:
    bool m_Active;
    int m_Index;
    AVCodecContext* m_pCodecContext;

    MediaType m_MediaType;

    FramerateCounter m_LastFramerateCounter;
    FramerateCounter m_AvgFramerateCounter;
    BitrateCounter m_BitrateCounter;
    Statistics m_Statistics;
    Clock m_Clock;

};

} // namespace mf
} // namespace surveon

#endif // SMF_AVI_VIDEO_STREAM_SOURCE_H


