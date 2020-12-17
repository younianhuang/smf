#ifndef GLVR_PREDEFINED_SHADERS_H
#define GLVR_PREDEFINED_SHADERS_H

#include "GLVRRenderEnginePrerequisites.h"

namespace surveon
{
namespace glvr
{

class PredefindeShaders
{
public:
    static String getDefualtVertexShaderSource(void);

    static String getDefaultFragmentShaderSource(void);

    static String getRGBTextureFragmentShaderSource(void);

    static String getYUV420TextureFragmentShader(void);

};


} // namespace glvr
} // namespace surveon


#endif // GLVR_PREDEFINED_SHADERS_H

