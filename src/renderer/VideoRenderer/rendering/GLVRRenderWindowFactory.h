#ifndef GLVR_RENDER_WINDOW_FACTORY_H
#define GLVR_RENDER_WINDOW_FACTORY_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRRenderWindow.h"
#include "GLVRGLError.h"

namespace surveon
{
namespace glvr
{

class RenderWindowFactory
{
public:
    virtual ~RenderWindowFactory(void) {}

    virtual RenderWindow* create(mf::WindowHandle handle) = 0;

    virtual RenderWindow* create(uint32 width, uint32 height, const String& title) = 0;

};

typedef std::unique_ptr<RenderWindowFactory> RenderWindowFactoryPtr;

RenderWindowFactoryPtr createRenderWindowFactory(void);

} // namespace glvr
} // namespace surveon



#endif // GLVR_RENDER_WINDOW_FACTORY_H


