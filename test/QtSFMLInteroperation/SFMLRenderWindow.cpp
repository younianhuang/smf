#include "SFMLRenderWindow.h"
#include <QResizeEvent>
#include <QMessageBox>
#include <sstream>
#include <iostream>

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

SFMLRenderWindow::SFMLRenderWindow(QWidget *parent) :
    QWidget(parent),
    m_Initialized(false),
    m_pFont(0),   
    //m_Thread(&SFMLRenderWindow::ProcessData, this),
    m_TextureUpdated(false),
    m_Run(false),
    m_FPS(0.0f),
    m_WindowSizeChanged(false)
{
        // Setup some states to allow direct rendering into the widget
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);

        // Set strong focus to enable keyboard events to be received
        setFocusPolicy(Qt::StrongFocus);
}

SFMLRenderWindow::~SFMLRenderWindow()
{

}

void SFMLRenderWindow::setFont(sf::Font& font)
{
    m_pFont = &font;
}

void SFMLRenderWindow::Update()
{
    OnUpdate();
}

void SFMLRenderWindow::Stop()
{
    m_Run = false;

    //m_Thread.wait();
}

void SFMLRenderWindow::OnInit()
{    
    m_VideoPane.setPosition(0, 0);
    m_VideoPane.setSize(sf::Vector2f(width(), height()));
    m_VideoPane.setFillColor(sf::Color::White);

    m_RectShape.setPosition(10, 50);
    m_RectShape.setSize(sf::Vector2f(32, 32));
    m_RectShape.setFillColor(sf::Color::White);
    m_RectShape.setOutlineThickness(2);
    m_RectShape.setOutlineColor(sf::Color::Yellow);

    m_ConvexShape.setPointCount(5);
    m_ConvexShape.setPoint(0, sf::Vector2f(112, 183));
    m_ConvexShape.setPoint(1, sf::Vector2f(211, 109));
    m_ConvexShape.setPoint(2, sf::Vector2f(309, 184));
    m_ConvexShape.setPoint(3, sf::Vector2f(271, 315));
    m_ConvexShape.setPoint(4, sf::Vector2f(154, 314));
    m_ConvexShape.setFillColor(sf::Color(0,0,0, 128));
    m_ConvexShape.setOutlineThickness(5);
    m_ConvexShape.setOutlineColor(sf::Color::Yellow);

    if(m_Image.loadFromFile("..//resource//texture_1080p.jpg"))
    {
        m_Texture.create(m_Image.getSize().x, m_Image.getSize().y);

        m_Texture.update( m_Image);

        m_VideoPane.setTexture(&m_Texture);

        m_RectShape.setTexture(&m_Texture);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Load Texture failed.");
        msgBox.exec();
    }

    if(m_pFont)
    {
        m_Text.setFont(*m_pFont);
        m_Text.setColor(sf::Color::Green);
        m_Text.setCharacterSize(12);
        m_Text.setStyle(sf::Text::Bold);
        m_Text.setPosition(5, 0);
    }

    m_FrameStatistics.restart();

    m_RenderWindow.setActive(false);

    //m_Run = true;
    //m_Thread.launch();
}

void SFMLRenderWindow::OnUpdate()
{
    sf::Lock lock(m_Mutex);

    m_RenderWindow.setActive(true);

    if(m_RenderWindow.isOpen())
    {
/*
        if(m_WindowSizeChanged)
        {

            m_Mutex.lock();

            m_RenderWindow.setSize(
                        sf::Vector2u(m_WindowSize.width(),
                                     m_WindowSize.height()));

            m_WindowSizeChanged = false;

            m_Mutex.unlock();
        }
*/
        m_RenderWindow.clear(sf::Color::Black);

        m_Texture.update( m_Image);

        m_RenderWindow.draw(m_VideoPane);

        m_RenderWindow.draw(m_RectShape);

        m_RenderWindow.draw(m_ConvexShape);

        m_RenderWindow.draw(m_Text);

        m_RenderWindow.display();

        m_FrameStatistics.update();

        float fps = m_FrameStatistics.getFPS();
        if(m_FPS != fps)
        {
            std::stringstream sstream;

            sstream << "fps="<< fps;

            m_Text.setString(sstream.str());

            m_FPS = fps;
        }

        m_TextureUpdated = false;
    }

    m_RenderWindow.setActive(false);
}

void SFMLRenderWindow::ProcessData()
{
    while(m_Run)
    {
        //m_Mutex.lock();
        if(!m_TextureUpdated)
        {
            m_Texture.update(m_Image);

            m_TextureUpdated = true;
        }

        //m_Mutex.unlock();

        sf::sleep(sf::milliseconds(10));
    }
}

QPaintEngine* SFMLRenderWindow::paintEngine() const
{
    return 0;
}

void SFMLRenderWindow::showEvent(QShowEvent*)
{
    if(!m_Initialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
        m_RenderWindow.create(sf::WindowHandle(winId()));

        // Let the derived class do its specific stuff
        OnInit();

        m_Initialized = true;
    }
}

void SFMLRenderWindow::paintEvent(QPaintEvent*)
{
    // Let the derived class do its specific stuff
    //OnUpdate();
}

void SFMLRenderWindow::resizeEvent (QResizeEvent * event)
{
    //m_Mutex.lock();

    m_WindowSize = event->size();

    m_RenderWindow.setActive(true);

    m_RenderWindow.setSize(sf::Vector2u(event->size().width(), event->size().height()));

    m_RenderWindow.resetGLStates();

    m_WindowSizeChanged = true;

    m_RenderWindow.setActive(false);
    //m_Mutex.unlock();

/*
    m_RenderWindow.setActive(true);

    m_RenderWindow.setSize(sf::Vector2u(event->size().width(), event->size().height()));

    //OnUpdate();

    m_RenderWindow.setActive(false);
*/
    //QWidget::resizeEvent(event);
}

