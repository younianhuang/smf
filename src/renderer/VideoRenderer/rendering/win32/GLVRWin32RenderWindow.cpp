
#include "GLVRWin32RenderWindow.h"
#include "GLVRD3D9Manager.h"

namespace surveon
{
namespace glvr
{
bool Win32RenderWindow::ms_WindowRegistered = false;

 const wchar_t* g_ClassName  = L"GRVRWindow";


void Win32RenderWindow::registerWindowClass()
{
    WNDCLASSW windowClass;
    windowClass.style         = 0;
    windowClass.lpfnWndProc   = &Win32RenderWindow::wndProc;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = GetModuleHandle(NULL);
    windowClass.hIcon         = NULL;
    windowClass.hCursor       = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName  = NULL;
    windowClass.lpszClassName = g_ClassName;

    ATOM atom = RegisterClassW(&windowClass);
    if(atom ==  0)
    {
        GLVR_THROW_EXCEPT(GLVRInternalErrorException, "Failed to register window " <<  g_ClassName);
    }
}

void Win32RenderWindow::unregisterWindowClass()
{
    UnregisterClassW(g_ClassName, GetModuleHandleW(NULL));
}

LRESULT CALLBACK Win32RenderWindow::wndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(handle, message, wParam, lParam);
}

//===================================================================================

Win32RenderWindow::Win32RenderWindow() :
    RenderWindow(),
    m_pGLContext(nullptr),
    m_WindowHandle(NULL)
{
    //GLVR_LOG_INFO( "Win32RenderWindow 0x" << this << " is created");    
}

Win32RenderWindow::~Win32RenderWindow()
{
    close();

    //GLVR_LOG_INFO( "Win32RenderWindow 0x" << this << " is destroyed");
}

void Win32RenderWindow::open(uint32 width, uint32 height, const String& title)
{
    m_WindowHandle = createWindow(width, height, title);

    m_pGLContext = WGLContextManager::getInstance().createGLContext(m_WindowHandle);

    WGLContextManager::getInstance().makeCurrent(m_pGLContext);

    RenderWindow::open(width, height, title);
}

void Win32RenderWindow::open(mf::WindowHandle handle)
{
    m_WindowHandle = (HWND)handle;

    m_pGLContext = WGLContextManager::getInstance().createGLContext(m_WindowHandle);

    WGLContextManager::getInstance().makeCurrent(m_pGLContext);

    RenderWindow::open(handle);    
}

void Win32RenderWindow::close(void)
{
    if(m_Open)
    {
        releaseResources();

        if(m_Priamry)
        {
            D3D9Manager::destroyInstance();
        }

        WGLContextManager::getInstance().destroyGLContext(m_pGLContext);

        m_pGLContext = nullptr;

        if(m_OwnWindow)
        {
            DestroyWindow(m_WindowHandle);

            // NOTE:
            // This is a just quick and dirty way to  unregister window class
            // Because now primary only create its window.
            unregisterWindowClass();
        }

        m_Open = false;
    }
}


void Win32RenderWindow::swapBuffer(void)
{
    m_pGLContext->swapBuffer();
}

void Win32RenderWindow::makeCurrent(bool active)
{
    if(active)
        WGLContextManager::getInstance().makeCurrent(m_pGLContext);
    else
        WGLContextManager::getInstance().makeCurrent(nullptr);
}

HardwarePixelBufferSharedPtr Win32RenderWindow::createHardwarePixelBuffer(uint32 width, uint32 height)
{
    return D3D9Manager::getInstancePtr()->getD3D9DeviceManager()->createSurface(width, height);
}

IDeviceManager* Win32RenderWindow::getDeviceManager(void)
{
    return D3D9Manager::getInstancePtr()->getD3D9DeviceManager();
}

/*
bool Win32RenderWindow::isOpen (void) const
{
    return m_Open;
}
*/

void Win32RenderWindow::hide(void)
{
    ShowWindow(m_WindowHandle, SW_HIDE);
}

//===================================================================

HWND Win32RenderWindow::createWindow(uint32 width, uint32 height, const String& title)
{
    if(!ms_WindowRegistered)
    {
        registerWindowClass();
        ms_WindowRegistered = true;
    }

    // TODO:
    // convert title to wide char string
    HWND hWnd = CreateWindowW(
                g_ClassName,
                L"Title",
                WS_CAPTION | WS_MINIMIZEBOX,
                0, 0, width, height,
                NULL,
                NULL,
                GetModuleHandle(NULL),
                NULL);

    if(hWnd ==  NULL)
    {
        GLVR_THROW_EXCEPT(GLVRInternalErrorException, "Failed to create window " <<  title);
    }

    return hWnd;
}

} // namespace glvr
} // namespace surveon


