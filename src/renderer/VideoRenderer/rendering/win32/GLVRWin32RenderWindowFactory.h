
#ifndef GLVR_WIN32_RENDER_WINDOW_FACTORY_H
#define GLVR_WIN32_RENDER_WINDOW_FACTORY_H

#include "GLVRWin32Prerequisites.h"
#include "GLVRWin32RenderWindow.h"
#include "../GLVRRenderWindowFactory.h"

namespace surveon
{
namespace glvr
{

class Win32RenderWindowFactory : public RenderWindowFactory
{
public:
    virtual ~Win32RenderWindowFactory(void) {}

    virtual RenderWindow* create(mf::WindowHandle handle);

    virtual RenderWindow* create(uint32 width, uint32 height, const String& title);
};


} // namespace glvr
} // namespace surveon

#endif // GLVR_WIN32_RENDER_WINDOW_H

