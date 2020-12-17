#include "GLVRGLError.h"

namespace surveon
{
namespace glvr
{

const String& GLError::getString(GLenum errorCode)
{    
    static std::map<GLenum, String> errorStringMap =
    {
        { GL_INVALID_ENUM, "GL_INVALID_ENUM"},
        { GL_INVALID_VALUE, "GL_INVALID_VALUE"},
        { GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
        { GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
        { GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
        { GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
        { GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"}
    };

    return errorStringMap[errorCode];
}


String GLError::toString(GLenum errorCode)
{
    switch(errorCode)
    {
    case GL_INVALID_ENUM:
        return String("GL_INVALID_ENUM");
    case GL_INVALID_VALUE:
        return String("GL_INVALID_VALUE");
    case GL_INVALID_OPERATION:
        return String("GL_INVALID_OPERATION");
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return String("GL_INVALID_FRAMEBUFFER_OPERATION");
    case GL_OUT_OF_MEMORY:
        return String("GL_OUT_OF_MEMORY");
    case GL_STACK_UNDERFLOW:
        return String("GL_STACK_UNDERFLOW");
    case GL_STACK_OVERFLOW:
        return String("GL_STACK_OVERFLOW");
    }

    return String("GL_UNKNOW_ERROR");
}

} // namespace glvr
} // namespace surveon



