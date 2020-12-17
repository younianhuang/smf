#include "GLVRMaterial.h"

namespace surveon
{
namespace glvr
{

Material::Material()
{

}

Material::~Material()
{

}

void Material::setProgram(GPUProgramSharedPtr program)
{
    m_pProgram = program;
}

GPUProgramSharedPtr Material::getProgram(void)
{
    return m_pProgram;
}


void Material::setTexture(int texUnit, TextureSharedPtr pTexture)
{
    if(static_cast<size_t>(texUnit) >= m_TextureUnitStates.size())
    {
        GLVR_THROW_EXCEPT(InvalidParametersException, "Texture Unit State " << texUnit << " does not exist" );
    }

    m_TextureUnitStates[texUnit].pTexture = pTexture;
}


TextureSharedPtr Material::getTexture(int texUnit)
{
    if(static_cast<size_t>(texUnit) >= m_TextureUnitStates.size())
    {
        GLVR_THROW_EXCEPT(InvalidParametersException, "Texture Unit " << texUnit << " does not bind any texture" );
    }

    return m_TextureUnitStates[texUnit].pTexture;
}


void Material::addTextureUnitState(const TextureUnitState& textureUnitState)
{
    m_TextureUnitStates.push_back(textureUnitState);
}

Material::TextureUnitStates& Material::getTextureUnitStates(void)
{
    return m_TextureUnitStates;
}

/*
void Material::bind(void)
{
    //if(m_pProgram)
    //    m_pProgram->bind();

    for(auto pair : m_TextureUnits)
    {
        pair.second->bind(pair.first);
    }
}
*/

} // namespace glvr
} // namespace surveon



