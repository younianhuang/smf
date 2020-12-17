#include "GLVRShader.h"

namespace surveon
{
namespace glvr
{

Shader::Shader(ShaderType type) :
    m_ShaderType(type),
    m_ShaderObjectId(0),
    m_Status(ST_UNLOADED)
{

}

Shader::~Shader()
{
    if(m_ShaderObjectId)
    {
        GLVR_GL_TRACE(glDeleteShader(m_ShaderObjectId));
    }

}

void Shader::loadFromString(const String& source)
{
    m_ShaderObjectId = GLVR_GL_CHECK(glCreateShader(m_ShaderType));

    const GLchar* cstrSource = source.c_str();
    GLVR_GL_CHECK(glShaderSource(m_ShaderObjectId, 1, &cstrSource, NULL));

    m_Status = ST_LOADED;
}

void Shader::compile(void)
{
    GLVR_GL_CHECK(glCompileShader(m_ShaderObjectId));

    GLint status = 0;
    GLVR_GL_CHECK(glGetShaderiv(m_ShaderObjectId, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        GLVR_GL_CHECK(glGetShaderiv(m_ShaderObjectId, GL_INFO_LOG_LENGTH, &infoLogLength));

        std::unique_ptr<GLchar[]> infoLog(new GLchar[infoLogLength + 1]);
        GLVR_GL_CHECK(glGetShaderInfoLog(m_ShaderObjectId, infoLogLength, NULL, infoLog.get()));

        GLVR_THROW_EXCEPT(GLVROpenGLSLCompileErrorException,
                          "Shader [" << m_ShaderObjectId << "] compile error:  " << infoLog.get());
    }

    m_Status = ST_COMPILED;
}

} // namespace glvr
} // namespace surveon



