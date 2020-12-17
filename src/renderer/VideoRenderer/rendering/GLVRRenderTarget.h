#ifndef GLVR_RENDER_TARGET_H
#define GLVR_RENDER_TARGET_H


#include "GLVRRenderable.h"
#include "GLVRViewPort.h"
#include "GLVRRenderQueue.h"

namespace surveon
{
namespace glvr
{

struct RenderStates
{
    GPUProgramSharedPtr pProgram;
    Material::TextureUnitStates textureUnitStates;
};


class RenderTarget
{
public:
    RenderTarget(RenderStates* pRenderStates);

    virtual ~RenderTarget(void);

    virtual void setViewport(const mf::FloatRect& rect);

    virtual const Viewport& getViewport(void) const;

    virtual void render(Renderable* pRenderable);

    virtual void addRenderable(Renderable* pRenderable);

    virtual void removeRenderable(Renderable* pRenderable);

    virtual void removeAllRenderables(void);

    virtual void swapBuffer(void) = 0;

    virtual void render(void);

    virtual HardwarePixelBufferSharedPtr createHardwarePixelBuffer(uint32 width, uint32 height) = 0;

    virtual IDeviceManager* getDeviceManager(void) = 0;

protected:
    void applyViewport(void);

    void bindMaterial(MaterialSharedPtr pMaterial);

    void bindGPUProgram(GPUProgramSharedPtr pProgram);

    void unbindGPUProgram(void);

    void bindTextures(Material::TextureUnitStates& textureUnitStates);

private:    
    std::list<Renderable*> m_RenderList;

    RenderQueue m_RenderQueue;

    Viewport m_ViewPort;

    RenderStates* m_pRenderStates;

    bool m_Updated;
};

} // namespace glvr
} // namespace surveon



#endif // RENDERTARGET_H
