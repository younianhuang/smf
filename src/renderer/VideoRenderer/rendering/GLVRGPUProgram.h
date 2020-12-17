#ifndef GLVR_GPUPROGRAM_H
#define GLVR_GPUPROGRAM_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"
#include "GLVRShader.h"

namespace surveon
{
namespace glvr
{

class GPUProgram
{
public:
    GPUProgram(void);
    ~GPUProgram(void);

    void addShader(ShaderSharedPtr pShader);

    void link(void);

    void bind(void);

    void unbind(void);

    void setParameter(const String& name, int value);

    void setParameters(void);

    void setAttribLocation(const String& attributeName, int location);

    int getAttribLocation(const String& attributeName);

private:
    void attachShaders(void);
    void detachShaders(void);

    void setParameterCommand(const String& name, int value);

private:
    GLuint m_ProgramObjectId;

    std::vector<ShaderSharedPtr> m_Shaders;

    mutable mf::AsyncCommandQueue m_CommandQueue;

    //std::vector<mf::AsyncCommandPtr> m_ParameterCommands;
};

typedef std::shared_ptr<GPUProgram>GPUProgramSharedPtr;

} // namespace glvr
} // namespace surveon



#endif // GLVR_GPUPROGRAM_H

