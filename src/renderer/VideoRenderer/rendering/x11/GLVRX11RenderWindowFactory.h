
#ifndef GLVR_X11_RENDER_WINDOW_FACTORY_H
#define GLVR_X11_RENDER_WINDOW_FACTORY_H


#include "GLVRX11RenderWindow.h"
#include "../GLVRRenderWindowFactory.h"

namespace surveon
{
namespace glvr
{

class X11RenderWindowFactory : public RenderWindowFactory
{
public:
    virtual ~X11RenderWindowFactory(void) {}

    virtual RenderWindow* create(mf::WindowHandle handle);

    virtual RenderWindow* create(uint32 width, uint32 height, const String& title);
};


} // namespace glvr
} // namespace surveon

#endif // GLVR_X11_RENDER_WINDOW_FACTORY_H

