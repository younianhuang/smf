#ifndef VIEWTILE_H
#define VIEWTILE_H

#include "CommonConfig.h"
#include <QWidget>
#include <SMF.h>
#include <GLVR.h>

class ViewTile : public QWidget
{
    Q_OBJECT

public:
    explicit ViewTile(QWidget *parent = 0);
    ~ViewTile();

public:
    float getRendererFPS() const;
    float getDecoderFPS() const;
    size_t getDecoderBitrate() const;
    float getSourceFPS() const;
    size_t getSourceBitrate() const;
    void setFileSource(const QString& filename);
    void closeFile(void);
    void openFile(void);
    void stopSource(void);
    void startSource(void);
    void pauseSource(void);
    void resumeSource(void);
    void setHardwareDecode(bool hardwareDecode);
    QString getCodec(void);
    QString getVideoResolution(void);

#ifdef Bool
    #undef Bool // "#define Bool Int" in Xlib.h
#endif

public slots:
    void setRenderingVideo(bool renderingVideo);
    void setDecodeVideo(bool decodeVideo);
    void setDownScaling(bool downScaling);
    void setTimingControl(bool timingControl);
    void setRate(float rate = FLOAT_MAX);

protected:
    virtual void closeEvent ( QCloseEvent * event );
    virtual void resizeEvent (QResizeEvent * event);
    virtual void showEvent(QShowEvent*);

    void processVideo(void);

    void createVideoBuffers(void);
    void destroyVideoBuffers(void);

    void setupDisplayRect(const QSize& size);

    surveon::mf::IVideoBuffer*getNextVideoBuffer(void);

    void decodeVideo(void);
    void renderVideo(void);

    void startPlayVideo();

private:
    void setDecoderHardwareDecode(void);

private:
    bool m_Run;
    bool m_FirstPacket;
    bool m_HardwareDecode;

    std::atomic<bool> m_RenderingVideo;
    std::atomic<bool> m_DecodeVideo;
    std::atomic<bool> m_DownScaling;
    std::atomic<int> m_WindowWidth;
    std::atomic<int> m_WindowHeight;
    float m_Rate;

    surveon::glvr::IVideoRenderer* m_pVideoRenderer;
    surveon::glvr::IVideoPane* m_pVideoPane;
    surveon::mf::IClock* m_pClock;
    surveon::mf::IPresentationClock* m_pPresentationClock;
    bool m_TimingControl;

    surveon::mf::IMediaSource* m_pSource;
    surveon::mf::IMediaTransform* m_pDecoder;
    std::vector< surveon::mf::IVideoBuffer*> m_VideoBuffers;
    size_t m_VideoBufferIndex;
    surveon::mf::IMediaBuffer* m_pCurrentCompressedVideoBuffer;
    surveon::mf::IVideoBuffer* m_pCurrentDecodedVideoBuffer;
    surveon::mf::Thread m_Thread;

    surveon::mf::AsyncCommandQueue::SyncControl m_SyncControl;

    QString m_FileSource;
};

#endif // VIEWTILE_H
