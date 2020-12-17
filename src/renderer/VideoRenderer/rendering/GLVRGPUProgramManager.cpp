#include "GLVRGPUProgramManager.h"
#include "GLVRPredefinedShaders.h"
#include "GLVRVertexData.h"

namespace surveon
{
namespace glvr
{


//===============================================================================================

GPUProgramManager::GPUProgramManager()
{

}

GPUProgramManager::~GPUProgramManager()
{
    GLVR_LOG_INFO("destroy GPUProgramManager.");
    shutdown();
}

void GPUProgramManager::initialize(void)
{
    createPredefinedShaders();
    createPredefinedProgramrs();
}

void GPUProgramManager::shutdown(void)
{
    destroyPredefinedShaders();
    destroyPredefinedPrograms();
}

ShaderSharedPtr GPUProgramManager::createShaderFromString(const String& name, Shader::ShaderType type,
                                                          const String& source)
{
    if(m_ShaderMap.find(name) != m_ShaderMap.end())
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Shader " << name << "already exists!");
    }

    ShaderSharedPtr pShader(new Shader(type));

    pShader->loadFromString(source);

    pShader->compile();

    m_ShaderMap.insert(ShaderMap::value_type(name, pShader));

    return pShader;

}

ShaderSharedPtr GPUProgramManager::getShader(const String& name)
{
    auto it = m_ShaderMap.find(name);
    if(it == m_ShaderMap.end())
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Shader " << name << "does not exist!");
    }

    return it->second;
}

void GPUProgramManager::destroyShader(const String& name)
{
    auto it = m_ShaderMap.find(name);
    if(it == m_ShaderMap.end())
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Shader " << name << "does not exist!");
    }

    m_ShaderMap.erase(it);
}

/*
GPUProgramSharedPtr GPUProgramManager::createGPUProgram(void)
{
    GPUProgramSharedPtr pGPUProgram(new GPUProgram);

    return pGPUProgram;
}
*/

void GPUProgramManager::createDefaultGPUProgram(void)
{
    m_pDefaultProgram.reset(new GPUProgram);

    m_pDefaultProgram->addShader(m_pDefaultVertexShader);

    m_pDefaultProgram->addShader(m_pDefaultFragmentShader);

    m_pDefaultProgram->setAttribLocation("inPosition", VertexData::VERTEX_LOCATION_POSITION);
    m_pDefaultProgram->setAttribLocation("inTexCoord", VertexData::VERTEX_LOCATION_TEX_COORD);
    m_pDefaultProgram->setAttribLocation("inColor", VertexData::VERTEX_LOCATION_VERTEX_COLOR);

    m_pDefaultProgram->link();
}


void GPUProgramManager::createRGBTextureGPUProgram(void)
{
    m_pRGBTextureProgram.reset(new GPUProgram);

    m_pRGBTextureProgram->addShader(m_pDefaultVertexShader);

    m_pRGBTextureProgram->addShader(m_pRGBTextureFragmentShader);

    m_pRGBTextureProgram->setAttribLocation("inPosition", VertexData::VERTEX_LOCATION_POSITION);
    m_pRGBTextureProgram->setAttribLocation("inTexCoord", VertexData::VERTEX_LOCATION_TEX_COORD);
    m_pRGBTextureProgram->setAttribLocation("inColor", VertexData::VERTEX_LOCATION_VERTEX_COLOR);

    m_pRGBTextureProgram->link();       
}

void GPUProgramManager::createYUV420TextureGPUProgram(void)
{
    m_pYUV420TextureProgram.reset(new GPUProgram);

    m_pYUV420TextureProgram->addShader(m_pDefaultVertexShader);

    m_pYUV420TextureProgram->addShader(m_pYUV420TextureFragmentShader);

    m_pYUV420TextureProgram->setAttribLocation("inPosition", VertexData::VERTEX_LOCATION_POSITION);
    m_pYUV420TextureProgram->setAttribLocation("inTexCoord", VertexData::VERTEX_LOCATION_TEX_COORD);
    m_pYUV420TextureProgram->setAttribLocation("inColor", VertexData::VERTEX_LOCATION_VERTEX_COLOR);

    m_pYUV420TextureProgram->link();
}

void GPUProgramManager::createPredefinedShaders(void)
{
    m_pDefaultVertexShader = createPredefinedShader(
                Shader::ST_VERTEX_SHADER, PredefindeShaders::getDefualtVertexShaderSource());

    m_pDefaultFragmentShader = createPredefinedShader(
                Shader::ST_FRAGMENT_SHADER, PredefindeShaders::getDefaultFragmentShaderSource());

    m_pRGBTextureFragmentShader = createPredefinedShader(
            Shader::ST_FRAGMENT_SHADER, PredefindeShaders::getRGBTextureFragmentShaderSource());

    m_pYUV420TextureFragmentShader = createPredefinedShader(
            Shader::ST_FRAGMENT_SHADER, PredefindeShaders::getYUV420TextureFragmentShader());
}


void GPUProgramManager::destroyPredefinedShaders(void)
{
    m_pDefaultVertexShader.reset();

    m_pDefaultFragmentShader.reset();

    m_pRGBTextureFragmentShader.reset();

    m_pYUV420TextureFragmentShader.reset();
}

void GPUProgramManager::createPredefinedProgramrs(void)
{
    createDefaultGPUProgram();
    createRGBTextureGPUProgram();
    createYUV420TextureGPUProgram();
}

void GPUProgramManager::destroyPredefinedPrograms(void)
{
    m_pDefaultProgram.reset();
    m_pRGBTextureProgram.reset();
    m_pYUV420TextureProgram.reset();
}

ShaderSharedPtr GPUProgramManager::createPredefinedShader(Shader::ShaderType type, const String& source)
{
    ShaderSharedPtr pShader(new Shader(type));

    pShader->loadFromString(source);

    pShader->compile();

    return pShader;
}

} // namespace glvr
} // namespace surveon
