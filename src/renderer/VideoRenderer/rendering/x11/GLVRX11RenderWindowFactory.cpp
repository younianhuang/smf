
#include "GLVRX11RenderWindowFactory.h"


namespace surveon
{
namespace glvr
{

RenderWindow* X11RenderWindowFactory::create(mf::WindowHandle handle)
{
    X11RenderWindow* pRenderWindow = new X11RenderWindow();
    pRenderWindow->open(handle);
    return pRenderWindow;
}

RenderWindow* X11RenderWindowFactory::create(uint32 width, uint32 height, const String& title)
{
    X11RenderWindow* pRenderWindow = new X11RenderWindow();
    pRenderWindow->open(width, height, title);
    return pRenderWindow;
}

//=====================================================================================

RenderWindowFactoryPtr createRenderWindowFactory(void)
{
    return RenderWindowFactoryPtr(new X11RenderWindowFactory);
}


} // namespace glvr
} // namespace surveon


