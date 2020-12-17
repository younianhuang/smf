#ifndef GLVR_GL_CONTEXT_H
#define GLVR_GL_CONTEXT_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"

namespace surveon
{
namespace glvr
{

class GLContext
{
public:
    GLContext() {}
    virtual ~GLContext()    {}

    virtual void makeCurrent(bool current = true) = 0;
};


} // namespace glvr
} // namespace surveon



#endif // GLVR_GL_CONTEXT_H


