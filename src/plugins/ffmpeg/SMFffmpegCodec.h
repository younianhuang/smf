#ifndef SMF_FFMPEG_CODEC_H
#define SMF_FFMPEG_CODEC_H

#include "SMFffmpegException.h"
#include "SMFFFmpegHelper.h"

namespace surveon
{
namespace mf
{

class ffmpegCodec
{
public:
    ffmpegCodec(MediaSubType type);
    ~ffmpegCodec();

    void process(IMediaBuffer* pBuffer);

    void getOutput(IMediaBuffer* pBuffer);

    //Status processInput(IMediaBuffer* pBuffer);

    //Status processOutput(IMediaBuffer* pBuffer);

private:
    AVCodecContext* m_pAVCodecContext;

    AVCodec* m_pAVCodec;

    AVFrame* m_pAVFrame;
};

} // namespace mf
} // namespace surveon


#endif // SMF_FFMPEG_CODEC_H



