#include "GLVRWGLContext.h"

namespace surveon
{
namespace glvr
{

WGLContext::WGLContext(HWND hWnd) :
    m_HWND(hWnd)
{
    m_HDC = GetDC(hWnd);

    createContext();
}

WGLContext::WGLContext(WGLContext* pWglContext,  HWND hWnd)
{
    m_HDC = GetDC(hWnd);

    createContext();

    if (!wglShareLists(pWglContext->m_GLRC, m_GLRC))
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Failed to share OpenGL Content " << SMF_ADDR_PREFIX(m_GLRC));
    }
}

WGLContext::~WGLContext()
{
    if (m_GLRC)
   {       
        GLVR_LOG_DEBUG("Destroy GLContent " << SMF_ADDR_PREFIX(m_GLRC));
       wglDeleteContext(m_GLRC);
   }

   // Destroy the device context
   if (m_HDC)
   {
       ReleaseDC(m_HWND, m_HDC);
    }
}

void WGLContext::makeCurrent(bool current)
{
    if(current)
    {
        GLVR_LOG_TRACE("Make GLContent " << SMF_ADDR_PREFIX(m_GLRC) << " current");
        if(wglMakeCurrent(m_HDC, m_GLRC) == FALSE)
        {
            GLVR_LOG_ERROR("Fail to make GLContex " << SMF_ADDR_PREFIX(m_GLRC) << " to current");
        }
    }
    else
    {
        GLVR_LOG_DEBUG("Clear GLContent 0x" << SMF_ADDR_PREFIX(m_GLRC));
        if(wglMakeCurrent(NULL, NULL) == FALSE)
        {
            GLVR_LOG_ERROR("Fail to clear contex GLContex " << SMF_ADDR_PREFIX(m_GLRC));
        }
    }
}

void WGLContext::swapBuffer(void)
{
    if (m_HDC && m_GLRC)
           SwapBuffers(m_HDC);
}

//==================================================================
void WGLContext::createContext(void)
{
    // Retrive current desktop video mode
    DEVMODE videoMode;
    videoMode.dmSize = sizeof(videoMode);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &videoMode);

    // Setup a pixel format descriptor from the rendering settings
    PIXELFORMATDESCRIPTOR descriptor;
    ZeroMemory(&descriptor, sizeof(descriptor));
    descriptor.nSize        = sizeof(descriptor);
    descriptor.nVersion     = 1;
    descriptor.iLayerType   = PFD_MAIN_PLANE;
    descriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    descriptor.iPixelType   = PFD_TYPE_RGBA;
    descriptor.cColorBits   = videoMode.dmBitsPerPel;
    descriptor.cDepthBits   = 0;
    descriptor.cStencilBits = 0;
    descriptor.cAlphaBits   = 8;

    // Get the pixel format that best matches our requirements
    int bestFormat = ChoosePixelFormat(m_HDC, &descriptor);
    if (bestFormat == 0)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Failed to choose pixel format");
    }

    if (!SetPixelFormat(m_HDC, bestFormat, &descriptor))
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Failed to set pixel format");
    }

    m_GLRC = wglCreateContext(m_HDC);
    if (m_GLRC == NULL)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Failed to craete OpenGL Content");
    }

    GLVR_LOG_DEBUG("GLContent " << SMF_ADDR_PREFIX(m_GLRC) << " is created");
}

} // namespace glvr
} // namespace surveon



