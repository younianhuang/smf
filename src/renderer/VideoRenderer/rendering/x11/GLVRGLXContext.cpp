#include "GLVRGLXContext.h"

namespace surveon
{
namespace glvr
{


GLXContext::GLXContext(Display* pDisplay, int screen, Window window):
    m_pDisplay(pDisplay),
    m_Screen(screen),
    m_Window(window)
{    
    createContext(NULL);
}

GLXContext::GLXContext(GLXContext* pGLXContext, Display* pDisplay, int screen, Window window):
    m_pDisplay(pDisplay),
    m_Screen(screen),
    m_Window(window)
{    
    createContext(pGLXContext);
}


GLXContext::~GLXContext()
{
    glXDestroyContext(m_pDisplay, m_NativeGLXContext);
}

void GLXContext::makeCurrent(bool current)
{

    if(current)
    {
        if(glXMakeCurrent(m_pDisplay, m_Window, m_NativeGLXContext) == False)
        {
           GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Failed to make GL context current");
        }
    }
    else
    {        
        if(glXMakeCurrent(m_pDisplay, None, NULL) == False)
        {
           GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Failed to make GL context current");
        }
    }

}

void GLXContext::swapBuffer(void)
{
    if(m_Window)
      glXSwapBuffers(m_pDisplay, m_Window);
}

//==================================================================
void GLXContext::createContext(GLXContext* pGLXContext)
{
    XVisualInfo* visualInfo = NULL;

    // Retrieve the attributes of the target window
    XWindowAttributes windowAttributes;
    if (XGetWindowAttributes(m_pDisplay, m_Window, &windowAttributes) == 0)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Failed to get the window attributes");
    }

    // Get its visual
    XVisualInfo tpl;
    //tpl.screen   = DefaultScreen(m_pDisplay);
    tpl.screen = m_Screen;
    tpl.visualid = XVisualIDFromVisual(windowAttributes.visual);
    int nbVisuals = 0;
    visualInfo = XGetVisualInfo(m_pDisplay, VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);

    ::GLXContext pShareList = pGLXContext ? pGLXContext->m_NativeGLXContext : NULL;

    // Create the context, using the target window's visual
    m_NativeGLXContext = glXCreateContext(m_pDisplay, visualInfo, pShareList, true);
    if (!m_NativeGLXContext)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException,
                         "Failed to create an OpenGL context for this window");
    }

    // Free the visual info
    XFree(visualInfo);

}

} // namespace glvr
} // namespace surveon



