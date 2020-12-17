
#ifndef GLVR_WGLCONTEXT_H
#define GLVR_WGLCONTEXT_H

#include "../GLVRGLContext.h"
#include "../GLVRGLContextManager.h"
#include "GLVRWin32Prerequisites.h"


namespace surveon
{
namespace glvr
{

class WGLContext : public GLContext
{
public:
    WGLContext(HWND hWnd);
    WGLContext(WGLContext* pWglContext,  HWND hWnd);
    virtual ~WGLContext();

    virtual void makeCurrent(bool current = true);

    virtual void swapBuffer(void);

private:
    void createContext(void);

private:
    HWND m_HWND;
    HDC     m_HDC;
    HGLRC   m_GLRC;
};


typedef GLContextManager<WGLContext> WGLContextManager ;

} // namespace glvr
} // namespace surveon

#endif // GLVR_WGLCONTEXT_H


