#include "GLVRRenderWindow.h"


namespace surveon
{
namespace glvr
{


RenderWindow::RenderWindow() :
    RenderTarget(&m_RenderStates),
    m_Priamry(false),
    m_Open(false),
    m_OwnWindow(false)
{
    GLVR_LOG_INFO( "RenderWindow " << SMF_ADDR_PREFIX(this) << " is created");
}


RenderWindow::~RenderWindow()
{        
    GLVR_LOG_INFO( "RenderWindow " << SMF_ADDR_PREFIX(this) << " is destroyed");
}

void RenderWindow::open(uint32 width, uint32 height, const String& title)
{
    m_OwnWindow = true;

    m_Open = true;
}

void RenderWindow::open(mf::WindowHandle handle)
{
    m_OwnWindow = false;

    m_Open = true;
}

/*

void RenderWindow::open(uint32 width, uint32 height, const String& title)
{
    m_SFMLWindow.create(sf::VideoMode(width, height), title, sf::Style::None);

    m_SFMLWindow.resetGLStates();

    m_SFMLWindow.setVerticalSyncEnabled(false);

    resetGLState();

    m_SFMLWindow.setActive(false);

    m_Open = true;
}

void RenderWindow::open(mf::WindowHandle handle)
{
    m_WindowHandle = handle;
    m_SFMLWindow.create(sf::WindowHandle(handle));
    m_SFMLWindow.resetGLStates();

    m_SFMLWindow.setVerticalSyncEnabled(false);

    resetGLState();

    m_SFMLWindow.setActive(false);

    m_Open = true;
}

void RenderWindow::close(void)
{    
    releaseResources();
    m_SFMLWindow.close();

    m_Open = false;
}

void RenderWindow::swapBuffer(void)
{
    m_SFMLWindow.display();
}

void RenderWindow::makeCurrent(bool active)
{
    if(active)
    {
        GLVR_LOG_DEBUG( "RenderWindow 0x" << this << " active");
    }
    else
    {
        GLVR_LOG_DEBUG( "RenderWindow 0x" << this << " inactive");
    }

    if(!m_SFMLWindow.setActive(active))
    {
        GLVR_THROW_EXCEPT(GLVRInternalErrorException, "fail to active render window");
    }

void RenderWindow::hide(void)
{
    m_SFMLWindow.setVisible(false);
}

*/
void RenderWindow::releaseResources(void)
{
    m_RenderStates.pProgram.reset();
    m_RenderStates.textureUnitStates.clear();
}




bool RenderWindow::isOpen (void) const
{
    return m_Open;
}

void RenderWindow::clearFrameBuffer(const Color& color)
{
    GLVR_GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
    GLVR_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    //GLVR_GL_CHECK(glClearDepth(1.0f));
    //GLVR_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderWindow::resetGLState(void)
{

    GLVR_GL_CHECK(glDisable(GL_CULL_FACE));
    GLVR_GL_CHECK(glDisable(GL_LIGHTING));
    GLVR_GL_CHECK(glDisable(GL_DEPTH_TEST));
    GLVR_GL_CHECK(glDisable(GL_ALPHA_TEST));
    GLVR_GL_CHECK(glEnable(GL_TEXTURE_2D));
    GLVR_GL_CHECK(glEnable(GL_BLEND));

    // Apply the blend mode, falling back to the non-separate versions if necessary
    if (GLEW_EXT_blend_func_separate)
    {
        GLVR_GL_CHECK(glBlendFuncSeparateEXT(
            GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
            GL_ONE, GL_ONE));
    }
    else
    {
        GLVR_GL_CHECK(glBlendFunc(
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA));
    }

    if (GLEW_EXT_blend_equation_separate)
    {
        GLVR_GL_CHECK(glBlendEquationSeparateEXT(
            GL_FUNC_ADD,
            GL_FUNC_ADD));
    }
    else
    {
        GLVR_GL_CHECK(glBlendEquation(GL_FUNC_ADD));
    }
}



} // namespace glvr
} // namespace surveon
