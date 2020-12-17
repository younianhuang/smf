
#include "GLVRX11WindowManager.h"


namespace surveon
{
namespace glvr
{

PFNGLXBINDTEXIMAGEEXTPROC       glXBindTexImageEXT = nullptr;
PFNGLXRELEASETEXIMAGEEXTPROC    glXReleaseTexImageEXT = nullptr;;


X11WindowManager::X11WindowManager():
    m_pDisplay(nullptr),
    m_ReferenceCount(0)
{

}

X11WindowManager::~X11WindowManager()
{

}

Display* X11WindowManager::openDisplay(void)
{
    if(!m_pDisplay)
    {
        m_pDisplay = XOpenDisplay(NULL);
        if(!m_pDisplay)
        {
            GLVR_THROW_EXCEPT(GLVRInternalErrorException, "Failed to open display");
        }

        glXBindTexImageEXT = (PFNGLXBINDTEXIMAGEEXTPROC)glXGetProcAddress((GLubyte *)"glXBindTexImageEXT");

        glXReleaseTexImageEXT = (PFNGLXRELEASETEXIMAGEEXTPROC)glXGetProcAddress((GLubyte *)"glXReleaseTexImageEXT");

        assert(glXBindTexImageEXT && glXReleaseTexImageEXT);
    }

    m_ReferenceCount++;

    return m_pDisplay;
}

void X11WindowManager::closeDisplay(Display* pDisplay)
{
    assert(m_pDisplay == pDisplay);

    if(--m_ReferenceCount == 0)
    {
        XCloseDisplay(m_pDisplay);

        m_pDisplay = nullptr;
    }
}


} // namespace glvr
} // namespace surveon

