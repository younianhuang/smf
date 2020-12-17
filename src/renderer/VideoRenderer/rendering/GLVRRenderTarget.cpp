#include "GLVRRenderTarget.h"

namespace surveon
{
namespace glvr
{


RenderTarget::RenderTarget(RenderStates* pRenderStates) :
    m_pRenderStates(pRenderStates),    
    m_Updated(true)
{
    assert(pRenderStates);
}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::setViewport(const mf::FloatRect& rect)
{
    m_ViewPort.setRect(rect);

    m_Updated = true;
}

const Viewport& RenderTarget::getViewport(void) const
{
    return m_ViewPort;
}

void RenderTarget::addRenderable(Renderable* pRenderable)
{
    if(!pRenderable)
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Invalid null pointer!");
    }

    m_RenderList.push_back(pRenderable);
}

void RenderTarget::removeRenderable(Renderable* pRenderable)
{
    auto it = std::find(m_RenderList.begin(), m_RenderList.end(), pRenderable);

    if(it != m_RenderList.end())
        m_RenderList.erase(it);
}

void RenderTarget::render(void)
{
    applyViewport();

    // re-arrange Renderable order by z order
    for(Renderable* pRenderable : m_RenderList)
    {
        if(pRenderable->isShow())
        {
            m_RenderQueue.addRenderable(pRenderable->getZOrder(), pRenderable);
        }
    }

    RenderQueue::Iterator it = m_RenderQueue.getRenderGroupMapIterator();
    while(it.hasMore())
    {
        RenderQueue::RenderGroup& renderGroup = it.getNext();

        while(renderGroup.hasMore())
        {
            Renderable* pRenderable = renderGroup.getNext();

            render(pRenderable);

        }
    }
}

void RenderTarget::removeAllRenderables(void)
{
    m_RenderList.clear();
}

void RenderTarget::render(Renderable* pRenderable)
{
    if(pRenderable->isShow())
    {
        pRenderable->update();

        bindMaterial(pRenderable->getMaterial());

        RenderOperation& renderOperation = pRenderable->getRenderOperation();
        renderOperation.apply();
    }
}

//=======================================================================================
void RenderTarget::applyViewport(void)
{
    if(m_Updated)
    {
        GLVR_GL_CHECK(glViewport(
                          static_cast<GLsizei>(m_ViewPort.getLeft()),
                          static_cast<GLsizei>(m_ViewPort.getTop()),
                          static_cast<GLsizei>(m_ViewPort.getRight()),
                          static_cast<GLsizei>(m_ViewPort.getBottom())));

        for(auto pRenderable : m_RenderList)
        {
            pRenderable->notifyViewportChanged();
        }

        m_Updated = false;
    }
}

void RenderTarget::bindMaterial(MaterialSharedPtr pMaterial)
{
    if(pMaterial)
    {
        bindGPUProgram(pMaterial->getProgram());

        bindTextures(pMaterial->getTextureUnitStates());
    }
    else
    {
        unbindGPUProgram();
    }
}

void RenderTarget::bindTextures(Material::TextureUnitStates& textureUnitStates)
{
    if(m_pRenderStates->textureUnitStates.size() < textureUnitStates.size())
    {
        m_pRenderStates->textureUnitStates.resize(textureUnitStates.size());
    }

    size_t textureUnit=0;
    for( ; textureUnit<textureUnitStates.size(); textureUnit++)
    {
        TextureUnitState& textureUnitState = textureUnitStates[textureUnit];

        if(textureUnitState.pTexture != m_pRenderStates->textureUnitStates[textureUnit].pTexture)
        {
            textureUnitState.pTexture->bind(static_cast<int32>(textureUnit));
            m_pRenderStates->textureUnitStates[textureUnit] = textureUnitState;
        }
    }

    for( ; textureUnit<m_pRenderStates->textureUnitStates.size();  textureUnit++)
    {
        TextureSharedPtr& pTexture  = m_pRenderStates->textureUnitStates[textureUnit].pTexture;
        if(pTexture)
        {
            pTexture->unbind(static_cast<int32>(textureUnit));
            pTexture.reset();
        }
    }
}

void RenderTarget::bindGPUProgram(GPUProgramSharedPtr pProgram)
{
    if(pProgram)
    {
        if(m_pRenderStates->pProgram != pProgram)
            pProgram->bind();

        pProgram->setParameters();

        m_pRenderStates->pProgram = pProgram;
    }
    else
    {
        unbindGPUProgram();
    }
}

void RenderTarget::unbindGPUProgram(void)
{
    if(m_pRenderStates->pProgram)
        m_pRenderStates->pProgram->unbind();

    m_pRenderStates->pProgram.reset();
}

} // namespace glvr
} // namespace surveon


