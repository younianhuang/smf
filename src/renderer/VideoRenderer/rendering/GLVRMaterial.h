#ifndef GLVR_MATERIAL_H
#define GLVR_MATERIAL_H


#include "GLVRGPUProgram.h"
#include "GLVRTexture.h"

namespace surveon
{
namespace glvr
{

struct TextureUnitState
{
    TextureSharedPtr pTexture;
};

class Material
{
public:
    typedef std::vector<TextureUnitState> TextureUnitStates;

public:
    Material();
    ~Material();

    void setProgram(GPUProgramSharedPtr program);

    GPUProgramSharedPtr getProgram(void);

    void setTexture(int texUnit, TextureSharedPtr pTexture);

    TextureSharedPtr getTexture(int texUnit);

    void addTextureUnitState(const TextureUnitState& textureUnitState);

    TextureUnitStates& getTextureUnitStates(void);

    size_t getNumOfTextureUnitStates(void) const
    {
        return m_TextureUnitStates.size();
    }

private:
    GPUProgramSharedPtr m_pProgram;

    //std::map<int,TextureSharedPtr> m_TextureUnits;
    std::vector<TextureUnitState>  m_TextureUnitStates;
};

typedef std::shared_ptr<Material> MaterialSharedPtr;

} // namespace glvr
} // namespace surveon



#endif // GLVR_MATERIAL_H
