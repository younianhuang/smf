

#include "RenderingWindow.h"
#include <QResizeEvent>
#include <QMessageBox>
#include <framework/SMFPrimitives.h>
#include <iostream>
#include "CommonConfig.h"

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

#define WIDTH  1920
#define HEIGHT 1080

//using namespace surveon;
//using namespace surveon::mf;
using surveon::glvr::VideoRendererManager;
using surveon::mf::PlaneInfo;
using surveon::mf::uint32;
using surveon::mf::uint16;


surveon::glvr::ITexture* RenderingWindow::ms_pTexture = nullptr;

RenderingWindow::RenderingWindow(QWidget *parent, size_t videoBufferSize,
                                 surveon::mf::MediaSubType pixelFormat,
                                 ViewPane::Resolution resolution,
                                 bool textureUpload) :
    QWidget(parent),
    m_TextureUpload(textureUpload),
    m_Initialized(false),
    m_Run(false),
    m_MouseMoved(false),
    m_pBuffer(nullptr),
    m_VideoBufferSize(videoBufferSize),
    m_VideoBufferIndex(0),
    m_Bandwidth(0),
    m_BandwidthSum(0),
    m_MousePosition(QPoint()),
    m_PixelFormat(pixelFormat),
    m_LastTime(0),
    m_pVideoRenderer(nullptr),
    m_pVideoPane(nullptr),
    m_pFocusRenderable(nullptr),
    m_Resolution(resolution),
    m_pFocusButton(nullptr),

// XVideo Renderer
#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)
    m_pXVideoRenderer(new XVideoRenderer(this->winId())),
#endif

    m_pCanvasSprite(nullptr),
    m_pCanvas(nullptr),
    m_EnableCanvas(false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);
}

RenderingWindow::~RenderingWindow()
{
    free(m_pBuffer);

// XVideo Renderer
#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)
    free(m_pXVideoRenderer);
#endif
}

//===============================================================================================

void RenderingWindow::createPIP()
{
    if(m_pVideoPane)
    {
        surveon::glvr::IVideoPane* pVideoPanePIP =  m_pVideoRenderer->createVideoPane();
        pVideoPanePIP->shareStreamFrom(m_pVideoPane);
        m_VideoPanePIPs.push_back(pVideoPanePIP);
        pVideoPanePIP->setDestinationRect(surveon::mf::FloatRect(30, 30, 130, 130));
        pVideoPanePIP->setBorderColor(surveon::glvr::Color::Yellow);
        pVideoPanePIP->setBorderThickness(3);
        getFocus(pVideoPanePIP);
    }
}

void RenderingWindow::createRectangle()
{
    if(m_pVideoPane)
    {
        surveon::glvr::IRetangleShape* pRectangle = m_pVideoRenderer->createRetangleShape(400, 300, 700 , 500);
        m_Rectangles.push_back(pRectangle);

        pRectangle->setColor(surveon::glvr::Color(0.0f, 1.0f, 0.0f, 0.6f));

        pRectangle->setOutlineColor(surveon::glvr::Color::Yellow);

        getFocus(pRectangle);
    }
}

void RenderingWindow::createPolygon()
{
    if(m_pVideoPane)
    {
        surveon::glvr::IConvexShape* pPolygon = m_pVideoRenderer->createConvexShape(5);
        m_Polygons.push_back(pPolygon);
        m_PolygonPointCounts.push_back(5);

        pPolygon->setColor(0.0f, 0.0f, 1.0f, 0.5f);

        pPolygon->setPoint(0, 112, 183);
        m_PolygonPoints.push_back(surveon::glvr::Vector2f(112, 183));
        pPolygon->setPoint(1, 211, 109);
        m_PolygonPoints.push_back(surveon::glvr::Vector2f(211, 109));
        pPolygon->setPoint(2, 309, 184);
        m_PolygonPoints.push_back(surveon::glvr::Vector2f(309, 184));
        pPolygon->setPoint(3, 271, 315);
        m_PolygonPoints.push_back(surveon::glvr::Vector2f(271, 315));
        pPolygon->setPoint(4, 154, 315);
        m_PolygonPoints.push_back(surveon::glvr::Vector2f(154, 315));

        pPolygon->setOutlineColor(surveon::glvr::Color::Yellow);

        pPolygon->setZOrder(1);

        getFocus(pPolygon);
    }
}

void RenderingWindow::createSprite()
{
    if(m_pVideoPane)
    {
        surveon::glvr::ISprite* pSprite = m_pVideoRenderer->createSprite(50, 50, 114 , 114);
        m_Sprites.push_back(pSprite);

        pSprite->setBorderThickness(3);
        pSprite->setBorderColor(surveon::glvr::Color::Yellow);

        pSprite->setTexture(ms_pTexture);


        getFocus(pSprite);
    }
}

void RenderingWindow::destroyRenderable()
{
    surveon::glvr::IVideoPane* pVideoPanePIP = dynamic_cast<surveon::glvr::IVideoPane*>(m_pFocusRenderable);
    surveon::glvr::IRetangleShape* pRectangle = dynamic_cast<surveon::glvr::IRetangleShape*>(m_pFocusRenderable);
    surveon::glvr::IConvexShape* pPolygon = dynamic_cast<surveon::glvr::IConvexShape*>(m_pFocusRenderable);
    surveon::glvr::ISprite* pSprite = dynamic_cast<surveon::glvr::ISprite*>(m_pFocusRenderable);

    if(pVideoPanePIP && m_VideoPanePIPs.size()>0)
    {
        m_pVideoRenderer->destroyVideoPane(pVideoPanePIP);
        m_VideoPanePIPs.remove(pVideoPanePIP);
        m_pFocusRenderable = nullptr;
    }
    else if(pRectangle && m_Rectangles.size()>0)
    {
        m_pVideoRenderer->destroyRetangleShape(pRectangle);
        m_Rectangles.remove(pRectangle);
        m_pFocusRenderable = nullptr;
    }
    else if(pPolygon && m_Polygons.size()>0)
    {
        m_pVideoRenderer->destroyConvexShape(pPolygon);
        m_Polygons.remove(pPolygon);
        m_pFocusRenderable = nullptr;
    }
    else if(pSprite && m_Sprites.size()>0)
    {
        m_pVideoRenderer->destroySprite(pSprite);
        m_Sprites.remove(pSprite);
        m_pFocusRenderable = nullptr;
    }
}

void RenderingWindow::createButton()
{
    if(m_pVideoPane)
    {
        GLVRPushButton* pButton = new GLVRPushButton(m_pVideoRenderer, QString("button %1").arg(m_Buttons.size()+1), this);
        m_Buttons.push_back(pButton);

        pButton->move((int)m_Buttons.size()*100, (int)m_Buttons.size()*100); // set initial position
        pButton->installEventFilter(this); // register callback functions
        //pButton->setWindowOpacity(0.75);

        // use built-in signals of QPushButton
        connect(pButton, SIGNAL(clicked()), this, SLOT(clickButton()));
        connect(pButton, SIGNAL(pressed()), this, SLOT(pressButton()));
        connect(pButton, SIGNAL(released()), this, SLOT(releaseButton()));

        // to make QWidget rendered with GLVR
        // replace Widget::show with GLVRWidget::showWidget
        pButton->showWidget();

        // set opacity
        pButton->setWidgetOpacity(0.6f);

        getFocus(pButton);
    }
}

void RenderingWindow::destroyButton()
{
    if(m_pFocusButton && m_Buttons.size()>0)
    {
        m_pFocusButton->close();
        m_Buttons.remove(m_pFocusButton);
        m_pFocusButton = nullptr;
    }
}

void RenderingWindow::showButtons()
{
    // show all buttons
    for(Buttons::iterator it=m_Buttons.begin(); it!=m_Buttons.end(); ++it)
    {
        (*it)->showWidget();
    }
}

void RenderingWindow::hideButton()
{
    // hide focused button
    if (m_pFocusButton)
    {
        // replace Widget::hide with GLVRWidget::hideWidget
        m_pFocusButton->hideWidget();
    }
}

void RenderingWindow::Update()
{
    processVideo();
}

float RenderingWindow::getFPS() const
{
#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_GLVR)
    return m_pVideoRenderer->getStatistics().lastFPS;
#elif (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)
    return m_pXVideoRenderer->getFPS();
#endif
}

size_t RenderingWindow::getBandwidth() const
{
    return m_Bandwidth;
}

void RenderingWindow::setPixelFormat(surveon::mf::MediaSubType pixelFormat)
{
    MutexLock(m_Mutex);
    m_PixelFormat = pixelFormat;
}

void RenderingWindow::setVideoBufferSize(size_t videoBufferSize)
{
    MutexLock(m_Mutex);
    m_VideoBufferSize = videoBufferSize;
}

void RenderingWindow::setResolution(ViewPane::Resolution resolution)
{
    MutexLock(m_Mutex);
    m_Resolution = resolution;
}

//===============================================================================================

QPaintEngine* RenderingWindow::paintEngine() const
{
    return 0;
}

void RenderingWindow::showEvent(QShowEvent*)
{
    if(!m_Initialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Let the derived class do its specific stuff
        //m_RenderWindow.create(sf::WindowHandle(winId()));

        OnInit();

        m_Initialized = true;
    }
}

void RenderingWindow::paintEvent(QPaintEvent*)
{
    if(m_pVideoRenderer)
        m_pVideoRenderer->repaint();
}

void RenderingWindow::resizeEvent(QResizeEvent *event)
{
    if(m_pVideoRenderer)
        setupDisplayRect(event->size());
    QWidget::resizeEvent(event);
}

void RenderingWindow::closeEvent(QCloseEvent *)
{
    m_Run = false;
    m_Thread.join(); // stop thread
    OnUninit();
}

void RenderingWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_MousePosition = e->pos();

        for(VideoPanePIPs::iterator it=m_VideoPanePIPs.begin(); it!=m_VideoPanePIPs.end(); ++it)
        {
            if ((*it)->getDestinationRect().isInside(m_MousePosition.x(), m_MousePosition.y()))
            {
                getFocus(*it);
                return;
            }
        }

        for(Rectangles::iterator it=m_Rectangles.begin(); it!=m_Rectangles.end(); ++it)
        {
            if ((*it)->getRect().isInside(m_MousePosition.x(), m_MousePosition.y()))
            {
                getFocus(*it);
                return;
            }
        }

        for(Polygons::iterator it=m_Polygons.begin(); it!=m_Polygons.end(); ++it)
        {
            if (isInside((*it)->getBoundaryRect(), m_MousePosition.x(), m_MousePosition.y()))
            {
                getFocus(*it);
                return;
            }
        }

        for(Sprites::iterator it=m_Sprites.begin(); it!=m_Sprites.end(); ++it)
        {
            if ((*it)->getRect().isInside(m_MousePosition.x(), m_MousePosition.y()))
            {
                getFocus(*it);
                return;
            }
        }
    }
}

void RenderingWindow::mouseMoveEvent(QMouseEvent *e)
{
    surveon::glvr::IVideoPane* pVideoPanePIP = dynamic_cast<surveon::glvr::IVideoPane*>(m_pFocusRenderable);
    surveon::glvr::IRetangleShape* pRectangle = dynamic_cast<surveon::glvr::IRetangleShape*>(m_pFocusRenderable);
    surveon::glvr::IConvexShape* pPolygon = dynamic_cast<surveon::glvr::IConvexShape*>(m_pFocusRenderable);
    surveon::glvr::ISprite* pSprite = dynamic_cast<surveon::glvr::ISprite*>(m_pFocusRenderable);

    if (pVideoPanePIP && pVideoPanePIP->getDestinationRect().isInside(m_MousePosition.x(), m_MousePosition.y()))
    {
        QPoint tmp = e->pos() - m_MousePosition;
        surveon::mf::FloatRect rect = pVideoPanePIP->getDestinationRect();
        pVideoPanePIP->setDestinationRect(surveon::mf::FloatRect(
                rect.left+tmp.x(), rect.top+tmp.y(), rect.right+tmp.x(), rect.bottom+tmp.y()));
        m_MousePosition = e->pos();
    }
    else if (pRectangle && pRectangle->getRect().isInside(m_MousePosition.x(), m_MousePosition.y()))
    {
        QPoint tmp = e->pos() - m_MousePosition;
        surveon::mf::FloatRect rect = pRectangle->getRect();
        pRectangle->setRect(surveon::mf::FloatRect(
                rect.left+tmp.x(), rect.top+tmp.y(), rect.right+tmp.x(), rect.bottom+tmp.y()));
        m_MousePosition = e->pos();
    }
    else if (pPolygon && isInside(pPolygon->getBoundaryRect(), m_MousePosition.x(), m_MousePosition.y()))
    {
        QPoint tmp = e->pos() - m_MousePosition;

        // get target offset
        int offset;
        for(Polygons::iterator it=m_Polygons.begin(); it!=m_Polygons.end(); ++it)
        {
            if (pPolygon == *it)
                offset = std::distance(m_Polygons.begin(), it);
        }

        // advance iterator of PolygonPointCounts
        PolygonPointCounts::iterator itPointCountsTarget=m_PolygonPointCounts.begin();
        std::advance(itPointCountsTarget, offset);

        // advance iterator of PolygonPoints
        PolygonPoints::iterator itPoints=m_PolygonPoints.begin();
        for(PolygonPointCounts::iterator itPointCounts=m_PolygonPointCounts.begin(); itPointCounts!=itPointCountsTarget; ++itPointCounts)
        {
            std::advance(itPoints, *itPointCounts);
        }

        // set points
        for(int i=0; i<*itPointCountsTarget; i++)
        {
            (*itPoints).x += tmp.x();
            (*itPoints).y += tmp.y();
            pPolygon->setPoint(i, *itPoints++);
        }

        m_MousePosition = e->pos();
    }
    else if (pSprite && pSprite->getRect().isInside(m_MousePosition.x(), m_MousePosition.y()))
    {
        QPoint tmp = e->pos() - m_MousePosition;
        surveon::mf::FloatRect rect = pSprite->getRect();
        pSprite->setRect(surveon::mf::FloatRect(
                rect.left+tmp.x(), rect.top+tmp.y(), rect.right+tmp.x(), rect.bottom+tmp.y()));
        m_MousePosition = e->pos();
    }
}

void RenderingWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_MousePosition = QPoint();
    }
}

bool RenderingWindow::eventFilter(QObject *obj, QEvent *event)
{
    GLVRPushButton* pButton = dynamic_cast<GLVRPushButton*>(obj);

    if (event->type() == QEvent::MouseMove)
    {
        QPoint pos = QPoint(this->mapFromGlobal(QCursor::pos()).x(), this->mapFromGlobal(QCursor::pos()).y());
        pButton->move(pos - m_MousePosition);
        m_MouseMoved = true; // set flag
    }

    return QObject::eventFilter(obj, event);
}

void RenderingWindow::OnInit()
{
    // create video renderer which render to specified QWidet
    m_pVideoRenderer = VideoRendererManager::getInstancePtr()->createVideoRenderer();

    m_pVideoRenderer->initialize(surveon::mf::WindowHandle(winId()));

    // create video pane to render video
    m_pVideoPane = m_pVideoRenderer->createVideoPane();

    setupDisplayRect(this->size());

    // init clock
#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)
    m_pXVideoRenderer->initClock();
#endif

    // load texture
    if(!ms_pTexture)
    {
        ms_pTexture = VideoRendererManager::getInstancePtr()->createTexture();

        if (!m_TextureUpload)
        {
            ms_pTexture->loadFromFile("..//resource//insect.jpg");
        }
    }

    // create video buffer
    createVideoBuffer();

    if (!m_Run)
    {
        m_Thread = surveon::mf::Thread(&RenderingWindow::processVideo, this); // start thread
        m_Run = true;
    }
}

void RenderingWindow::OnUninit()
{
    m_pVideoRenderer->shutdown();

    VideoRendererManager::getInstancePtr()->destroyVideoRenderer(m_pVideoRenderer);
    m_pVideoRenderer = nullptr;

    if(m_pCanvas)
    {
        VideoRendererManager::getInstancePtr()->destroyTexture(m_pCanvas);
        m_pCanvas  = nullptr;
    }

    if (!m_VideoBuffers.empty())
    {
        for(VideoBuffers::iterator it=m_VideoBuffers.begin(); it!=m_VideoBuffers.end(); ++it)
        {
            surveon::mf::getSystemMemoryMediaBufferFactory()->destroyVideoBuffer(*it);
        }
        m_VideoBuffers.clear();
    }
}

void RenderingWindow::processVideo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // cushion for threading
    surveon::mf::MediaSubType lastRoundPixelFormat(m_PixelFormat);
    size_t lastRoundVideoBufferSize(m_VideoBufferSize);
    ViewPane::Resolution lastRoundResolution(m_Resolution);
    while(m_Run)
    {
#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_GLVR)
        // check if format changed
        {
            MutexLock(m_Mutex);

            if ((m_PixelFormat     != lastRoundPixelFormat)     ||
                (m_VideoBufferSize != lastRoundVideoBufferSize) ||
                (m_Resolution      != lastRoundResolution))
            {
                createVideoBuffer();
                lastRoundPixelFormat = m_PixelFormat;
                lastRoundVideoBufferSize = m_VideoBufferSize;
                lastRoundResolution = m_Resolution;
            }
        }

        if (m_TextureUpload)
        {
            if (m_PixelFormat == surveon::mf::SMF_VIDEO_FORMAT_RGBA)
            {
                ms_pTexture->upload(m_Image.bits());
                
                m_BandwidthSum += m_Image.width() * m_Image.height() * 4;
            }
            
            // update bandwidth
            surveon::mf::int64 currentTime = m_Clock.getMilliSeconds();
            surveon::mf::int64 elapsedTime = currentTime - m_LastTime;
            if (elapsedTime > 1000) // if timeout
            {
                m_Bandwidth = 1000 * m_BandwidthSum / elapsedTime; // in Bytes
                
                m_LastTime = currentTime;
                m_BandwidthSum = 0;
            }
        }
        else
        {
            surveon::mf::IVideoBuffer* pVideoBuffer = m_VideoBuffers[m_VideoBufferIndex];
            uploadVideoData(pVideoBuffer);

            // ring buffer
            if(++m_VideoBufferIndex >= m_VideoBuffers.size())
            {
                m_VideoBufferIndex = 0;
            }
        }
#elif (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)

        m_pXVideoRenderer->processVideo(m_pBuffer, WIDTH, HEIGHT,
                                        0, 0, WIDTH, HEIGHT,
                                        0, 0, this->width(), this->height());

#endif
    }
}

void RenderingWindow::uploadVideoData(surveon::mf::IVideoBuffer* pVideoBuffer, bool isForcedToMemcpy)
{
    if (!isForcedToMemcpy)
        pVideoBuffer->waitUntilFree(); // block until video data is rendered by video renderer
    else if (pVideoBuffer->isReferenced())
        return;

    size_t width = g_ResolutionSize[m_Resolution*2];
    size_t height = g_ResolutionSize[m_Resolution*2+1];

    const PlaneInfo&  planInfo = pVideoBuffer->lockPlane();

    //assert(m_VideoBuffers[i]->getPixelFormat() == m_PixelFormat);
    surveon::mf::MediaSubType pixelformat = pVideoBuffer->getVideoFormat();

    // upload video data
    if (pixelformat == surveon::mf::SMF_VIDEO_FORMAT_RGBA)
    {
        PlaneInfo::PlaneData planData = planInfo.getPlaneData(0);
        if (isForcedToMemcpy)
            memcpy(planData.pixels, m_Image.bits(), planData.memorySize);
    }
    else if (pixelformat == surveon::mf::SMF_VIDEO_FORMAT_YUV420)
    {
        // plane 0
        PlaneInfo::PlaneData yData = planInfo.getPlaneData(0);
        if (isForcedToMemcpy)
            memcpy(yData.pixels, m_pBuffer, yData.memorySize);

        // plane 1
        PlaneInfo::PlaneData uData = planInfo.getPlaneData(2);
        if (isForcedToMemcpy)
            memcpy(uData.pixels, m_pBuffer+width*height, uData.memorySize);

        // plane 2
        PlaneInfo::PlaneData vData = planInfo.getPlaneData(1);
        if (isForcedToMemcpy)
            memcpy(vData.pixels, m_pBuffer+width*height*5/4, uData.memorySize);
    }
    else if (pixelformat == surveon::mf::SMF_VIDEO_FORMAT_YV12)
    {
        // plane 0
        PlaneInfo::PlaneData yData = planInfo.getPlaneData(0);
        if (isForcedToMemcpy)
            memcpy(yData.pixels, m_pBuffer, yData.memorySize);

        // plane 1
        PlaneInfo::PlaneData uData = planInfo.getPlaneData(1);
        if (isForcedToMemcpy)
            memcpy(uData.pixels, m_pBuffer+width*height, uData.memorySize);

        // plane 2
        PlaneInfo::PlaneData vData = planInfo.getPlaneData(2);
        if (isForcedToMemcpy)
            memcpy(vData.pixels, m_pBuffer+width*height*5/4, uData.memorySize);
    }

    pVideoBuffer->unlockPlane();

    // upload video data to render
    m_pVideoPane->processData(pVideoBuffer);
}

void RenderingWindow::setupDisplayRect(const QSize& size)
{
    surveon::mf::FloatRect viewport(0, 0, size.width(), size.height());

    m_pVideoRenderer->setViewport(viewport);

    if(m_pVideoPane)
    {
        surveon::mf::FloatRect destRect(0, 0, size.width(), size.height());
        m_pVideoPane->setDestinationRect(destRect);

        m_pVideoPane->setSourceRect(surveon::mf::FloatRect(0.0, 0.0, 1.0, 1.0));

        if(m_EnableCanvas)
        {
            if(!m_pCanvasSprite)
                m_pCanvasSprite = m_pVideoRenderer->createSprite(
                            destRect.left, destRect.top, destRect.right, destRect.bottom);
            else
                m_pCanvasSprite->setRect(destRect);

            if(!m_pCanvas)
            {
                m_pCanvas = VideoRendererManager::getInstancePtr()->createTexture();
                m_pCanvasSprite->setTexture(m_pCanvas);
                m_pCanvas->create(destRect.getWidth(), destRect.getHeight());

                // create canvas sprite from pixel array
                const size_t size = 100 * 100;
                surveon::mf::uint32 pixels[size];
                for(size_t i=0; i<size; i++)
                {
                    pixels[i] = 0x96ff0000;
                }

                surveon::mf::IntRect rect(130, 150, 230, 250);
                m_pCanvas->upload((surveon::mf::uint8*)&pixels[0],
                        surveon::glvr::ITexture::PF_RGBA, &rect);

                // create canvas sprite from svg file
                int i = 0;
                foreach (QString fileName, QList<QString>() << "rectangle" << "circle" << "ellipse" << "polygon")
                {
                    QImageReader svg("..//resource//" + fileName + ".svg");
                    QImage image(svg.read().rgbSwapped());

                    int x = 20 + 120 * i++;
                    surveon::mf::IntRect rect2(x, 30, x+image.width(), 30+image.height());
                    m_pCanvas->upload((surveon::mf::uint8*)image.bits(),
                                      surveon::glvr::ITexture::PF_RGBA, &rect2);
                }
            }
        }
    }
}

void RenderingWindow::getFocus(surveon::glvr::IRenderable* pRenderable)
{
    // last one lose focus
    surveon::glvr::IVideoPane* pVideoPanePIP = dynamic_cast<surveon::glvr::IVideoPane*>(m_pFocusRenderable);
    surveon::glvr::IRetangleShape* pRectangle = dynamic_cast<surveon::glvr::IRetangleShape*>(m_pFocusRenderable);
    surveon::glvr::IConvexShape* pPolygon = dynamic_cast<surveon::glvr::IConvexShape*>(m_pFocusRenderable);
    surveon::glvr::ISprite* pSprite = dynamic_cast<surveon::glvr::ISprite*>(m_pFocusRenderable);

    if(pVideoPanePIP)
    {
        pVideoPanePIP->setColor(surveon::glvr::Color(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else if(pRectangle)
    {
        pRectangle->setOutlineThickness(0);
    }
    else if(pPolygon)
    {
        pPolygon->setOutlineThickness(0);
    }
    else if(pSprite)
    {
        pSprite->setColor(surveon::glvr::Color(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // this one get focus
    pVideoPanePIP = dynamic_cast<surveon::glvr::IVideoPane*>(pRenderable);
    pRectangle = dynamic_cast<surveon::glvr::IRetangleShape*>(pRenderable);
    pPolygon = dynamic_cast<surveon::glvr::IConvexShape*>(pRenderable);
    pSprite = dynamic_cast<surveon::glvr::ISprite*>(pRenderable);

    if(pVideoPanePIP)
    {
        pVideoPanePIP->setColor(surveon::glvr::Color(1.5f, 1.5f, 1.5f, 1.0f));
        m_pFocusRenderable = pVideoPanePIP;
    }
    else if(pRectangle)
    {
        pRectangle->setOutlineThickness(5);
        m_pFocusRenderable = pRectangle;
    }
    else if(pPolygon)
    {
        pPolygon->setOutlineThickness(5);
        m_pFocusRenderable = pPolygon;
    }
    else if(pSprite)
    {
        pSprite->setColor(surveon::glvr::Color(1.5f, 1.5f, 1.5f, 1.0f));
        m_pFocusRenderable = pSprite;
    }
}

void RenderingWindow::getFocus(GLVRPushButton* pButton)
{
    // last one lose focus
    if (m_pFocusButton)
    {
        m_pFocusButton->setStyleSheet("background-color: white");
    }

    // this one get focus
    if (pButton)
    {
        pButton->setStyleSheet("background-color: yellow");
        m_pFocusButton = pButton;
    }
}

bool RenderingWindow::isInside(surveon::mf::FloatRect rect, int x, int y) const
{
    return x>rect.left && x<rect.right && y>rect.top && y<rect.bottom;
}

void RenderingWindow::createVideoBuffer()
{
    // destroy the old one
    if (!m_VideoBuffers.empty())
    {
        if(m_pVideoRenderer)
            m_pVideoRenderer->flush();

        for(VideoBuffers::iterator it=m_VideoBuffers.begin(); it!=m_VideoBuffers.end(); ++it)
        {
            surveon::mf::getSystemMemoryMediaBufferFactory()->destroyVideoBuffer(*it);
        }
        m_VideoBuffers.clear();
    }
    if (m_pBuffer)
        free(m_pBuffer);

    // create a new one
    size_t width = g_ResolutionSize[m_Resolution*2];
    size_t height = g_ResolutionSize[m_Resolution*2+1];

      // load texture
    if (m_TextureUpload)
    {
        ms_pTexture->create((uint32)width, (uint32)height);
    }

      // load jpg to m_Image for RGB32
    m_Image.load(QString("..//resource//texture_%1.jpg").arg(QString::fromStdString(g_ResolutionStrings[m_Resolution])));
    QImage tmp = m_Image.rgbSwapped();
    m_Image.swap(tmp);

      // load yuv to m_pBuffer
    std::ifstream is(QString("..//resource//texture_%1.yuv")
                     .arg(QString::fromStdString(g_ResolutionStrings[m_Resolution])).toLatin1(), std::ifstream::binary);
    if (is)
    {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        // allocate memory:
        m_pBuffer = new char[length];
        char *tmpBuffer = new char[length];

        // read data as a block:
        is.read(tmpBuffer, length);

        memcpy(m_pBuffer, tmpBuffer, width*height); // yuv to yv12
        memcpy(m_pBuffer+width*height, tmpBuffer+width*height*5/4, width*height/4);
        memcpy(m_pBuffer+width*height*5/4, tmpBuffer+width*height, width*height/4);

        delete[] tmpBuffer;

        is.close();
    }

      // create buffers
    if (m_PixelFormat == surveon::mf::SMF_VIDEO_FORMAT_RGBA)
    {
        for(int i=0; i<m_VideoBufferSize; i++)
        {
            surveon::mf::IVideoBuffer* pVideoBuffer = surveon::mf::getSystemMemoryMediaBufferFactory()->
                    createVideoBuffer(m_Image.width(), m_Image.height(), surveon::mf::SMF_VIDEO_FORMAT_RGBA);
            m_VideoBuffers.push_back(pVideoBuffer);
        }
    }
    else if (m_PixelFormat == surveon::mf::SMF_VIDEO_FORMAT_YUV420)
    {
        for(int i=0; i<m_VideoBufferSize; i++)
        {
            surveon::mf::IVideoBuffer* pVideoBuffer = surveon::mf::getSystemMemoryMediaBufferFactory()->
                    createVideoBuffer((uint16)width, (uint16)height, surveon::mf::SMF_VIDEO_FORMAT_YUV420);
            m_VideoBuffers.push_back(pVideoBuffer);
        }
    }
    else if (m_PixelFormat == surveon::mf::SMF_VIDEO_FORMAT_YV12)
    {
        for(int i=0; i<m_VideoBufferSize; i++)
        {
            surveon::mf::IVideoBuffer* pVideoBuffer = surveon::mf::getSystemMemoryMediaBufferFactory()->
                    createVideoBuffer((uint16)width, (uint16)height, surveon::mf::SMF_VIDEO_FORMAT_YV12);
            m_VideoBuffers.push_back(pVideoBuffer);
        }
    }

    // put images
    for (int i=0; i<m_VideoBufferSize; i++)
    {
        uploadVideoData(m_VideoBuffers[i], true);
    }

    m_VideoBufferIndex = 0;
}

void RenderingWindow::clickButton()
{
    // reinitialize
    m_MousePosition = QPoint();
    m_MouseMoved = false;
}

void RenderingWindow::pressButton()
{
    QObject* obj = this->sender();
    GLVRPushButton* pButton = dynamic_cast<GLVRPushButton*>(obj);

    if (pButton)
    {
        // position relative to GLVRPushButton pos
        m_MousePosition = QPoint(this->mapFromGlobal(QCursor::pos()) - pButton->pos());
        getFocus(pButton);
    }
}

void RenderingWindow::releaseButton()
{
    QObject* obj = this->sender();
    GLVRPushButton* pButton = dynamic_cast<GLVRPushButton*>(obj);

    // show popup box
    if (!m_MouseMoved)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Event");
        msgBox.setText("Click " + pButton->text());
        msgBox.exec();
    }
}

void RenderingWindow::setEnableCanvas(bool enableCanvas)
{
    m_EnableCanvas = enableCanvas;

    if (m_pCanvasSprite)
    {
        if (enableCanvas)
        {
            m_pCanvasSprite->show();
        }
        else
        {
            m_pCanvasSprite->hide();
        }
    }
    else if (!m_pCanvasSprite && enableCanvas)
        setupDisplayRect(this->size()); // create canvas
}

