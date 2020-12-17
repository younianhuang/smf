
#include "GLVRPredefinedShaders.h"

namespace surveon
{
namespace glvr
{

String PredefindeShaders::getDefualtVertexShaderSource(void)
{
    return String (
                "#version 130\n"
                "in vec4 inPosition;\n"
                "in vec2 inTexCoord;"
                "in vec4 inColor;"
                "out vec2 texCoord0;"
                "smooth out vec4 vertexColor;"
                "void main()\n"
                "{\n"
                "   gl_Position = inPosition;\n"
                "   texCoord0 = inTexCoord;"
                "   vertexColor = inColor;"
                "}\n"
            );
}

String PredefindeShaders::getDefaultFragmentShaderSource(void)
{
    return String (
        "#version 130\n"
        "out vec4 fragColor;\n"
        "smooth in vec4 vertexColor;"
        "void main()\n"
        "{\n"
        "   fragColor = vertexColor;\n"
        "}\n"
    );
}

String PredefindeShaders::getRGBTextureFragmentShaderSource(void)
{
    return String (
                "#version 130\n"
                "uniform sampler2D sampler0;"
                "in vec2 texCoord0;"
                "smooth in vec4 vertexColor;"
                "out vec4 fragColor;\n"
                "void main()\n"
                "{\n"
                "   fragColor = texture2D(sampler0, texCoord0.st) * vertexColor;\n"
                "}\n"
            );
}

String PredefindeShaders::getYUV420TextureFragmentShader(void)
{
    return String(
                "#version 130\n"
                "uniform sampler2D sampler0;"
                "uniform sampler2D sampler1;"
                "uniform sampler2D sampler2;"
                "in vec2 texCoord0;"
                "smooth in vec4 vertexColor;"
                "out vec4 fragColor;\n"
                "void main()\n"
                "{\n"
                "   vec3 texel = vec3(texture2D(sampler0, texCoord0.st).r, "
                "                                 texture2D(sampler1, texCoord0.st).r - 0.50196, "
                "                                 texture2D(sampler2, texCoord0.st).r - 0.50196 );"
                "   fragColor.r = (texel.r + 1.402 * texel.b) * vertexColor.r ;\n"
                "   fragColor.g = (texel.r - 0.344 *  texel.g - 0.714 * texel.b) * vertexColor.g;\n"
                "   fragColor.b = (texel.r + 1.772 * texel.g) * vertexColor.b ;\n"
                "   fragColor.a = vertexColor.a;\n"
                "}\n"
            );
}


} // namespace glvr
} // namespace surveon


