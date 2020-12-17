#ifndef SMF_FFMPEG_DECODER_H
#define SMF_FFMPEG_DECODER_H

#include "SMFffmpegException.h"
#include "SMFFFmpegHelper.h"
#include "SMFHWAcceleration.h"

namespace surveon
{
namespace mf
{

class ffmpegVideoDecoder : public IMediaTransform
{
public:
    ffmpegVideoDecoder(AsyncCommandQueue* pCommandQueue);
    virtual ~ffmpegVideoDecoder();

    virtual const String& getTypeId(void) const;

    virtual void initialize(void);

    virtual void shutdown(void);

    virtual void setInputType(const MediaType& mediaType);

    virtual Status getInputStatus(void) const;

    virtual void processInput(IMediaBuffer* pBuffer);

    virtual void setOutputType(const MediaType& mediaType);

    virtual void getOutputStreamInfo(StreamInfo& streamInfo) const;

    virtual Status getOutputStatus(void) const;

    virtual void processOutput(IMediaBuffer* pBuffer);

    virtual void getStatistics(Statistics& stat) const;

    virtual Attributes* getAttributes(void);

    static const String& typeId(void)
    {
        return ms_TypeId;
    }

private:
    void initializeCommand();
    void shutdownCommand(void);

    void setInputTypeCommand(const MediaType& mediaType);
    Status getInputStatusCommand(void) const;
    void processInputCommand(IMediaBuffer* pBuffer);

    void getOutputStreamInfoCommand(StreamInfo* pStreamInfo) const;
    Status getOutputStatusCommand(void) const;
    void processOutputCommand(IMediaBuffer* pBuffer);

    void getStatisticsCommand(Statistics* pStat) const;

    void createCodec(bool enableHWAccel);
    void destroyCodec(void);

    void createHWAccel(void);

    void resetStatistics(void);
    void updateStatistics(void);

    void updateStreamInfo(void);

private:
    AVCodecContext* m_pAVCodecContext;

    AVCodec* m_pAVCodec;

    AVDictionary* m_AVOpts;

    AVFrame* m_pAVFrame;
    AVFrame* m_pAVFrameScaleOut;

    AVPacket m_Packet;

    SwsContext* m_pSwsContext;

    MediaType m_InputType;

    Status m_InputStatus;
    Status m_OutputStatus;
    StreamInfo m_StreamInfo;   
    Size m_OutputFrameSize;

    FramerateCounter m_LastFramerateCounter;
    FramerateCounter m_AvgFramerateCounter;
    BitrateCounter m_BitrateCounter;
    Statistics m_Statistics;
    Clock m_Clock;

    bool m_FirstPacket;

    AsyncCommandQueue* m_pCommandQueue;

    time_value m_Timestamp;

    static String ms_TypeId;

    Attributes m_Attributes;

    HWAcceleration* m_pHWAccel;

    IDeviceManager* m_pDeviceManager;
};

} // namespace mf
} // namespace surveon


#endif // SMF_FFMPEG_DECODER_H
