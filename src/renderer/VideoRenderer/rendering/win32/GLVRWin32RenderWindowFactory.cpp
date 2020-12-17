#include "GLVRWin32RenderWindowFactory.h"
#include "GLVRD3D9Manager.h"

namespace surveon
{
namespace glvr
{

RenderWindow* Win32RenderWindowFactory::create(mf::WindowHandle handle)
{
    Win32RenderWindow* pRenderWindow = new Win32RenderWindow();
    pRenderWindow->open(handle);
    return pRenderWindow;
}

RenderWindow* Win32RenderWindowFactory::create(uint32 width, uint32 height, const String& title)
{
    Win32RenderWindow* pRenderWindow = new Win32RenderWindow();
    pRenderWindow->open(width, height, title);
    return pRenderWindow;
}

//=====================================================================================

RenderWindowFactoryPtr createRenderWindowFactory(void)
{
    return RenderWindowFactoryPtr(new Win32RenderWindowFactory);
}

} // namespace glvr
} // namespace surveon

