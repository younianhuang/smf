

#include "RenderingWindow.h"
#include "ui_RenderingWindow.h"

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

using namespace surveon;
using namespace surveon::mf;
using namespace surveon::glvr;

RenderingWindow::RenderingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderingWindow),
    m_Initialized(false),
    m_pVideoRenderer(0)
{
    ui->setupUi(this);

    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);
}

RenderingWindow::~RenderingWindow()
{
    delete ui;   
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
    //m_pVideoRenderer->repaint();
}

void RenderingWindow::resizeEvent (QResizeEvent * event)
{

    QWidget::resizeEvent(event);
}

//===============================================================================================

void RenderingWindow::OnInit()
{
    // create video renderer which render to specified QWidet
    //m_pVideoRenderer = VideoRendererManager::getInstancePtr()->createVideoRenderer(winId());
    m_pVideoRenderer = VideoRendererManager::getInstancePtr()->createVideoRenderer();

    m_pVideoRenderer->initialize(mf::WindowHandle(winId()));

    // create video pane to render video
    m_pVideoPane = m_pVideoRenderer->createVideoPane();

    m_pVideoPane->setDestinationRect(mf::IntRect(0, 0, 320, 200));
/*
    // create a polygon

    ConvexPolygonShape* pPolygon = m_pVideoRenderer->createConvexPolygonShape();
    // set polygon vertex and it will be renderered when video data or polygon is updated

   pPolygon->setPointCount(5);
   pPolygon->setPoint(0, 10, 10);
   pPolygon->setPoint(1, 150, 10);
   pPolygon->setPoint(2, 120, 90);
   pPolygon->setPoint(3, 30, 100);
   pPolygon->setPoint(4, 0, 50);
*/
}

void RenderingWindow::OnUpdate()
{
    // push video data and video will be rendered in  video pane
    //m_pVideoPane->processSample(mediaSample);
}

void RenderingWindow::OnUninit()
{    
    VideoRendererManager::getInstancePtr()->shutdown();
}

