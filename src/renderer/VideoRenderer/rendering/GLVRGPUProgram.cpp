#include "GLVRGPUProgram.h"


namespace surveon
{
namespace glvr
{

GPUProgram::GPUProgram()
{
    m_ProgramObjectId = GLVR_GL_CHECK(glCreateProgram());
}

GPUProgram::~GPUProgram()
{
    GLVR_GL_TRACE(glDeleteProgram(m_ProgramObjectId));
}

void GPUProgram::addShader(ShaderSharedPtr pShader)
{
    m_Shaders.push_back(pShader);
}

void GPUProgram::link(void)
{
    attachShaders();

    GLVR_GL_CHECK(glLinkProgram(m_ProgramObjectId));

    GLint status = 0;
    GLVR_GL_CHECK(glGetProgramiv (m_ProgramObjectId, GL_LINK_STATUS, &status));
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        GLVR_GL_CHECK(glGetProgramiv(m_ProgramObjectId, GL_INFO_LOG_LENGTH, &infoLogLength));

        std::unique_ptr<GLchar[]> infoLog(new GLchar[infoLogLength + 1]);
        GLVR_GL_CHECK(glGetProgramInfoLog(m_ProgramObjectId, infoLogLength, NULL, infoLog.get()));

        detachShaders();

        GLVR_THROW_EXCEPT(GLVROpenGLSLLinkErrorException,
                          "Program [" << m_ProgramObjectId << "] link error:  " << infoLog.get());
    }

    detachShaders();
}

void GPUProgram::attachShaders(void)
{
    for(auto pShader : m_Shaders)
    {
        if(pShader->getStatus() == Shader::ST_LOADED)
        {
            pShader->compile();
        }

        GLVR_GL_CHECK(glAttachShader(m_ProgramObjectId, pShader->getId()));
    }
}

void GPUProgram::detachShaders(void)
{
    for(auto pShader : m_Shaders)
    {
        GLVR_GL_CHECK(glDetachShader(m_ProgramObjectId, pShader->getId()));
    }
}

void GPUProgram::bind(void)
{
     GLVR_GL_CHECK(glUseProgram(m_ProgramObjectId));     
}

void GPUProgram::unbind(void)
{
    GLVR_GL_CHECK(glUseProgram(0));
}

void GPUProgram::setParameter(const String& name, int value)
{
    m_CommandQueue.addCommand<void>(
                AsyncCommand::CT_SET,
                &GPUProgram::setParameterCommand, this, name, value);
}

void GPUProgram::setParameters(void)
{
    m_CommandQueue.executeCommands();
}

void GPUProgram::setAttribLocation(const String& attributeName, int location)
{
    GLVR_GL_CHECK(glBindAttribLocation(m_ProgramObjectId, location, attributeName.c_str()));
}

int GPUProgram::getAttribLocation(const String& attributeName)
{
    return glGetAttribLocation(m_ProgramObjectId, attributeName.c_str());
}

void GPUProgram::setParameterCommand(const String& name, int value)
{
    GLuint param = GLVR_GL_CHECK(glGetUniformLocation(m_ProgramObjectId, name.c_str()));
    GLVR_GL_CHECK(glUniform1i(param, value));
}

} // namespace glvr
} // namespace surveon



