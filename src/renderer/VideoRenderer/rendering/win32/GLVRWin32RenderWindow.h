
#ifndef GLVR_WIN32_RENDER_WINDOW_H
#define GLVR_WIN32_RENDER_WINDOW_H

#include "../GLVRRenderWindow.h"
#include "GLVRWin32Prerequisites.h"
#include "GLVRWGLContext.h"

namespace surveon
{
namespace glvr
{

class Win32RenderWindow : public RenderWindow
{
public:
    Win32RenderWindow();

    virtual ~Win32RenderWindow();

    virtual void open(uint32 width, uint32 height, const String& title);

    virtual void open(mf::WindowHandle handle);

    virtual void close(void);

    virtual void swapBuffer(void);

    virtual void makeCurrent(bool active);

    virtual HardwarePixelBufferSharedPtr createHardwarePixelBuffer(uint32 width, uint32 height);

    virtual IDeviceManager* getDeviceManager(void);

    //virtual bool isOpen(void) const;

    void hide(void);

    static HWND createWindow(uint32 width, uint32 height, const String& title);

private:
    static void registerWindowClass();

    static void unregisterWindowClass();

    static LRESULT CALLBACK wndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

    static bool ms_WindowRegistered;

protected:


private:

    HWND m_WindowHandle;

    WGLContext* m_pGLContext;

    //RenderStates m_RenderStates;
};

} // namespace glvr
} // namespace surveon

#endif // GLVR_WIN32_RENDER_WINDOW_H
