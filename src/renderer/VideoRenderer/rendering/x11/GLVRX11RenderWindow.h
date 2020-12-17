
#ifndef GLVR_X11_RENDER_WINDOW_H
#define GLVR_X11_RENDER_WINDOW_H

#include "../GLVRRenderWindow.h"
#include "GLVRX11Prerequisites.h"
#include "GLVRGLXContext.h"

namespace surveon
{
namespace glvr
{

class X11RenderWindow : public RenderWindow
{
public:
    X11RenderWindow();

    virtual ~X11RenderWindow();

    virtual void open(uint32 width, uint32 height, const String& title);

    virtual void open(mf::WindowHandle handle);

    virtual void close(void);

    virtual void swapBuffer(void);

    virtual void makeCurrent(bool active);

    virtual HardwarePixelBufferSharedPtr createHardwarePixelBuffer(uint32 width, uint32 height);

    virtual IDeviceManager* getDeviceManager(void);

    void hide(void);

private:

protected:


private:
    Display* m_pDisplay;

    Window m_Window;

    int m_Screen;

    GLXContext* m_pGLContext;

    XWindowAttributes m_WndAttrs;
};

} // namespace glvr
} // namespace surveon

#endif // GLVR_X11_RENDER_WINDOW_H
