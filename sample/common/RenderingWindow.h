#ifndef RENDERINGWINDOW_H
#define RENDERINGWINDOW_H

#include <QWidget>
#include <QImage>
#include <framework/SMFThread.h>
#include "GLVRWidget.h"
#include "ViewPane.h"

using surveon::mf::Mutex;
using surveon::mf::MutexLock;
using surveon::mf::String;

typedef GLVRWidget<QPushButton> GLVRPushButton;

class RenderingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RenderingWindow(QWidget *parent = 0, size_t videoBufferSize = 3,
                             surveon::mf::MediaSubType pixelFormat = surveon::mf::SMF_VIDEO_FORMAT_YV12,
                             ViewPane::Resolution resolution = ViewPane::RES_1080p,
                             bool textureUpload = false);
    ~RenderingWindow();

    void createPIP();
    void createRectangle();
    void createPolygon();
    void createSprite();
    void destroyRenderable();
    void createButton();
    void destroyButton();
    void showButtons();
    void hideButton();
    void Update();
    float getFPS() const;
    size_t getBandwidth() const;
    void setPixelFormat(surveon::mf::MediaSubType pixelFormat);
    void setVideoBufferSize(size_t videoBufferSize);
    void setResolution(ViewPane::Resolution resolution);

#ifdef Bool
    #undef Bool // "#define Bool Int" in Xlib.h
#endif

public slots:
    void setEnableCanvas(bool enableCanvas);

protected:
    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual bool eventFilter(QObject* obj, QEvent* event);

    void OnInit();
    void OnUninit();
    void processVideo();
    void uploadVideoData(surveon::mf::IVideoBuffer* pVideoBuffer, bool isForcedToMemcpy = 0);
    void setupDisplayRect(const QSize& size);
    void getFocus(surveon::glvr::IRenderable* pRenderable);
    void getFocus(GLVRPushButton* pButton);
    bool isInside(surveon::mf::FloatRect rect, int xPosition, int yPosition) const;
    void createVideoBuffer();

private slots:
    void clickButton();
    void pressButton();
    void releaseButton();

private:
    typedef std::list<surveon::glvr::IVideoPane*> VideoPanePIPs;
    typedef std::list<surveon::glvr::IRetangleShape*> Rectangles;
    typedef std::list<surveon::glvr::IConvexShape*> Polygons;
    typedef std::list<surveon::glvr::ISprite*> Sprites;
    typedef std::vector<surveon::mf::IVideoBuffer*> VideoBuffers;
    typedef std::list<int> PolygonPointCounts;
    typedef std::list<surveon::glvr::Vector2f> PolygonPoints;
    typedef std::list<GLVRPushButton*> Buttons;

    VideoPanePIPs m_VideoPanePIPs;
    Rectangles m_Rectangles;
    Polygons m_Polygons;
    Sprites m_Sprites;
    VideoBuffers m_VideoBuffers;
    PolygonPointCounts m_PolygonPointCounts;
    PolygonPoints m_PolygonPoints;
    Buttons m_Buttons;

    bool m_TextureUpload;
    bool m_Initialized;
    bool m_Run;
    bool m_MouseMoved;
    bool m_EnableCanvas;
    char* m_pBuffer;
    size_t m_VideoBufferSize;
    size_t m_VideoBufferIndex;
    std::atomic<size_t> m_Bandwidth;
    size_t m_BandwidthSum;
    QImage m_Image;
    QPoint m_MousePosition;
    surveon::mf::Thread m_Thread;
    surveon::mf::MediaSubType m_PixelFormat;
    surveon::mf::Mutex m_Mutex;
    surveon::mf::Clock m_Clock;
    surveon::mf::int64 m_LastTime;
    surveon::glvr::IVideoRenderer* m_pVideoRenderer;
    surveon::glvr::IVideoPane* m_pVideoPane;
    surveon::glvr::ISprite* m_pCanvasSprite;
    surveon::glvr::ITexture* m_pCanvas;
    surveon::glvr::IRenderable* m_pFocusRenderable;
    static surveon::glvr::ITexture* ms_pTexture;
    ViewPane::Resolution m_Resolution;
    GLVRPushButton* m_pFocusButton;

// XVideo Renderer
#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)
    XVideoRenderer *m_pXVideoRenderer;
#endif
};

#endif // RENDERINGWINDOW_H
