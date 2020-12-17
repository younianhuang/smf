#ifndef GLVR_GPU_PROGRAM_MANAGER_H
#define GLVR_GPU_PROGRAM_MANAGER_H

#include "GLVRGPUProgram.h"


namespace surveon
{
namespace glvr
{

class GPUProgramManager : public mf::Singleton<GPUProgramManager>
{
    friend class Singleton<GPUProgramManager>;

public:
    void initialize(void);

    void shutdown(void);

    ShaderSharedPtr createShaderFromString(const String& name, Shader::ShaderType type, const String& source);

    ShaderSharedPtr getShader(const String& name);

    void destroyShader(const String& name);

    GPUProgramSharedPtr getDefaultGPUProgram(void)
    {
        return m_pDefaultProgram;
    }

    GPUProgramSharedPtr getRGBTextureGPUProgram(void)
    {
        return m_pRGBTextureProgram;
    }

    GPUProgramSharedPtr getYUV420TextureGPUProgram(void)
    {
        return m_pYUV420TextureProgram;
    }

    //GPUProgramSharedPtr createGPUProgram(void);

private:
    GPUProgramManager();
    ~GPUProgramManager();

    void createPredefinedShaders(void);
    void destroyPredefinedShaders(void);

    void createPredefinedProgramrs(void);
    void destroyPredefinedPrograms(void);

    ShaderSharedPtr createPredefinedShader(Shader::ShaderType type, const String& source);

    void createDefaultGPUProgram(void);

    void createRGBTextureGPUProgram(void);

    void createYUV420TextureGPUProgram(void);

private:
    ShaderSharedPtr m_pDefaultVertexShader;

    ShaderSharedPtr m_pDefaultFragmentShader;

    ShaderSharedPtr m_pRGBTextureFragmentShader;

    ShaderSharedPtr m_pYUV420TextureFragmentShader;

    GPUProgramSharedPtr m_pDefaultProgram;

    GPUProgramSharedPtr m_pRGBTextureProgram;

    GPUProgramSharedPtr m_pYUV420TextureProgram;

    typedef std::map<String, ShaderSharedPtr> ShaderMap;
    ShaderMap m_ShaderMap;
};


} // namespace glvr
} // namespace surveon


#endif // GLVR_GPU_PROGRAM_MANAGER_H
