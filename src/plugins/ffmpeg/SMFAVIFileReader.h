

#ifndef SMF_AVI_FILE_READER_H
#define SMF_AVI_FILE_READER_H

#include "SMFffmpegException.h"
#include "SMFAVIVideoStreamSource.h"


namespace surveon
{
namespace mf
{

class MediaBufferRingPool
{
public:
    MediaBufferRingPool(size_t numOfBuffers, size_t bufferSize);

    IMediaBuffer* getToRead(void);
    IMediaBuffer* getToWrite(size_t size);

    bool isEmpty(void) const
    {
        return m_WriteIndex == m_ReadIndex;
    }

    bool isFull(void) const
    {
        size_t next = getNextIndex(m_WriteIndex);
        return next == m_ReadIndex;
    }

    size_t getNextIndex(const size_t index) const
    {
        size_t next = index+1;
        return next >= m_MediaBuffers.size() ? 0 : next;
    }

    void reset(void);

private:
    std::vector<IMediaBuffer*> m_MediaBuffers;

    size_t m_ReadIndex;
    size_t m_WriteIndex;
};


/**
 */
class AVIFileReader : public  IMediaSource
{
public:
    AVIFileReader(mf::AsyncCommandQueue* pCommandQueue);

    virtual ~AVIFileReader();

    virtual void initialize(const String& filename);

    virtual void shutdown(void);

    virtual void start(uint64 position = 0);

    virtual void stop(void);

    virtual void pause(void);

    virtual void resume(void);

    virtual StreamStatus getPacket(IMediaBuffer** ppMediaBuffer);

    virtual String getSource(void) const;

    virtual size_t getNumOfStream(void) const;

    virtual IStreamSource* getStreamSource(size_t index);

    void readPackets(void);

    StreamStatus getStreamStatus(void) const
    {
        return m_StreamStatus;
    }

    bool isBufferFulled(void) const
    {
        return m_MediaBufferPool.isFull();
    }

private:

    void openCommand(const String& filename);

    void closeCommand(void);

    void startCommand(uint64 position);

    void stopCommand(void);

    void pauseCommand(void);

    void resumeCommand(void);

    StreamStatus getPacketCommand(IMediaBuffer** ppMediaBuffer);

    const String& getSourceCommand(void) const;

    IStreamSource* getStreamSourceCommand(size_t index);

    void readPacket(IMediaBuffer* pMediaBuffer);

    void caculateTimestamp(IMediaBuffer* pMediaBuffer);

private:

    String m_Filename;

    AVFormatContext* m_pFormatContext;

    AVPacket m_Packet;

    typedef std::vector<IStreamSource*> StreamSources;
    StreamSources m_StreamSources;

    bool m_Initialized;
    mutable mf::AsyncCommandQueue* m_pCommandQueue;

    mf::Queue<IMediaBuffer*> m_Packets;

    StreamStatus m_StreamStatus;

    MediaBufferRingPool m_MediaBufferPool;

    int m_VideoStreamIndex;
    int m_AudioStreamIndex;
};

} // namespace mf
} // namespace surveon

#endif // SMF_AVI_FILE_READER_H

