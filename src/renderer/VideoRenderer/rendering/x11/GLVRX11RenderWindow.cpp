
#include "GLVRX11RenderWindow.h"
#include "GLVRX11WindowManager.h"
#include "GLVRGLXPixmap.h"

namespace surveon
{
namespace glvr
{

//===================================================================================

X11RenderWindow::X11RenderWindow() :
    RenderWindow(),
    m_pDisplay(nullptr),
    m_Screen(0),
    m_pGLContext(nullptr)

{
    memset(&m_WndAttrs, sizeof(XWindowAttributes), 0);
}

X11RenderWindow::~X11RenderWindow()
{
    close();
}

void X11RenderWindow::open(uint32 width, uint32 height, const String& title)
{
    // Open a connection with the X server
    m_pDisplay = X11WindowManager::getInstance().openDisplay();

    m_Screen = DefaultScreen(m_pDisplay);

    m_Window = XCreateWindow(m_pDisplay,
                             RootWindow(m_pDisplay, m_Screen),
                             0, 0,
                             width, height,
                             0,
                             DefaultDepth(m_pDisplay, m_Screen),
                             InputOutput,
                             DefaultVisual(m_pDisplay, m_Screen),
                             0, NULL);

    m_pGLContext = GLXContextManager::getInstance().createGLContext(m_pDisplay, m_Screen, m_Window);

    GLXContextManager::getInstance().makeCurrent(m_pGLContext);

    RenderWindow::open(width, height, title);

    XGetWindowAttributes(m_pDisplay, m_Window, &m_WndAttrs);

}

void X11RenderWindow::open(mf::WindowHandle handle)
{
    m_Window = handle;

    m_pDisplay = X11WindowManager::getInstance().openDisplay();

    m_Screen = DefaultScreen(m_pDisplay);

    m_pGLContext = GLXContextManager::getInstance().createGLContext(m_pDisplay, m_Screen, m_Window);

    GLXContextManager::getInstance().makeCurrent(m_pGLContext);

    RenderWindow::open(handle);

    XGetWindowAttributes(m_pDisplay, m_Window, &m_WndAttrs);
}

void X11RenderWindow::close(void)
{

    if(m_Open)
    {
        releaseResources();

        GLXContextManager::getInstance().destroyGLContext(m_pGLContext);
        m_pGLContext = nullptr;

        if(m_OwnWindow)
        {
            XDestroyWindow(m_pDisplay, m_Window);
            XFlush(m_pDisplay);
        }

        X11WindowManager::getInstance().closeDisplay(m_pDisplay);
        m_pDisplay = nullptr;

        m_Open = false;
    }
}


void X11RenderWindow::swapBuffer(void)
{
    m_pGLContext->swapBuffer();
}

void X11RenderWindow::makeCurrent(bool active)
{
    if(active)
        GLXContextManager::getInstance().makeCurrent(m_pGLContext);
    else
        GLXContextManager::getInstance().makeCurrent(nullptr);
}


void X11RenderWindow::hide(void)
{
    //ShowWindow(m_Window, SW_HIDE);
}

HardwarePixelBufferSharedPtr X11RenderWindow::createHardwarePixelBuffer(uint32 width, uint32 height)
{
    HardwarePixelBufferSharedPtr pPixelBuffer(
                new XPixmap(m_pDisplay, m_Screen, m_Window));

    static_cast<XPixmap*>(pPixelBuffer.get())->
            create(width, height, m_WndAttrs.depth);

    return pPixelBuffer;
}

IDeviceManager* X11RenderWindow::getDeviceManager(void)
{
    return nullptr;
}


} // namespace glvr
} // namespace surveon


