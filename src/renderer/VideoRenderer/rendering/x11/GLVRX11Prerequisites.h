
#ifndef GLVR_X11_PREREQUISITES_H
#define GLVR_X11_PREREQUISITES_H

#include "../GLVRRenderEnginePrerequisites.h"
#include <X11/Xlib.h>
#include <GL/glx.h>

namespace surveon
{
namespace glvr
{

extern PFNGLXBINDTEXIMAGEEXTPROC       glXBindTexImageEXT;
extern PFNGLXRELEASETEXIMAGEEXTPROC    glXReleaseTexImageEXT;


} // namespace glvr
} // namespace surveon

#endif // GLVR_X11_PREREQUISITES_H

