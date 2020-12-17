
#ifndef GLVR_GLXCONTEXT_H
#define GLVR_GLXCONTEXT_H

#include "../GLVRGLContext.h"
#include "../GLVRGLContextManager.h"
#include "GLVRX11Prerequisites.h"


namespace surveon
{
namespace glvr
{

class GLXContext : public GLContext
{
public:

    GLXContext(Display* pDisplay, int screen, Window window);

    GLXContext(GLXContext* pGLXContext, Display* pDisplay, int screen, Window window);

    virtual ~GLXContext();

    virtual void makeCurrent(bool current = true);

    virtual void swapBuffer(void);

private:
    void createContext(GLXContext* pGLXContext);

private:
    Display* m_pDisplay;

    int m_Screen;

    Window m_Window;

    ::GLXContext m_NativeGLXContext;
};


typedef GLContextManager<GLXContext> GLXContextManager;

} // namespace glvr
} // namespace surveon

#endif // GLVR_GLXCONTEXT_H


