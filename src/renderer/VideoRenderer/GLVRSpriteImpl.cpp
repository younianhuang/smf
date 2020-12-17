
#include "GLVRSpriteImpl.h"
#include "GLVRTextureImpl.h"

namespace surveon
{

namespace glvr
{


SpriteImpl::SpriteImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue, const mf::FloatRect& rect) :
    m_pCommandQueue(pCommandQueue),
    m_pRetangleShape(new RetangleShape(pRenderTarget, rect))
{
    GLVR_LOG_INFO( "Sprite "  << SMF_ADDR_PREFIX(this) << " is created");

    MaterialSharedPtr pMaterial(new Material);

    pMaterial->setProgram(GPUProgramManager::getInstance().getRGBTextureGPUProgram() );

    m_pRetangleShape->setMaterial(pMaterial);
}

SpriteImpl::~SpriteImpl()
{    
    delete m_pRetangleShape;

    GLVR_LOG_INFO( "Sprite "  << SMF_ADDR_PREFIX(this) << " is destroyed");
}

uint8 SpriteImpl::getZOrder(void) const
{
    return m_pCommandQueue->blockingCall<uint8>(
                AsyncCommand::CT_GET,
                &RetangleShape::getZOrder, m_pRetangleShape);
}

void SpriteImpl::setZOrder(uint8 zorder)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setZOrder, m_pRetangleShape,
                zorder);
}

void SpriteImpl::show(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::show, m_pRetangleShape);
}

void SpriteImpl::hide(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::hide, m_pRetangleShape);
}

bool SpriteImpl::isShow(void) const
{
    return m_pCommandQueue->blockingCall<bool>(
                AsyncCommand::CT_GET,
                &RetangleShape::isShow, m_pRetangleShape);
}

void SpriteImpl::setColor(const Color& color)
{
    return m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setFillColor, m_pRetangleShape,
                color);
}

void SpriteImpl::setBorderColor(const Color& color)
{
    return m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setOutlineColor, m_pRetangleShape,
                color);
}

void SpriteImpl::setBorderThickness(int thickness)
{
    return m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setOutlineThickness, m_pRetangleShape,
                thickness);
}

void SpriteImpl::setRect(const mf::FloatRect& rect)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setRect, m_pRetangleShape,
                rect);
}

mf::FloatRect SpriteImpl::getRect(void) const
{
    return m_pCommandQueue->blockingCall<mf::FloatRect>(
                AsyncCommand::CT_GET,
                &RetangleShape::getRect, m_pRetangleShape);
}

void SpriteImpl::setTexture(ITexture* pTexture)
{
    if(!pTexture)
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Input pointer is null !");
    }

    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &SpriteImpl::setTextureCommand, this,
                pTexture);
}

void SpriteImpl::setTextureRect(const mf::FloatRect& rect)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setTextureCoordRect, m_pRetangleShape,
                rect);
}

mf::FloatRect SpriteImpl::getBoundaryRect(void) const
{
    return m_pCommandQueue->blockingCall<mf::FloatRect>(
                AsyncCommand::CT_GET,
                &RetangleShape::getBoundaryRect, m_pRetangleShape);
}

//=============================================================================================

void SpriteImpl::setTextureCommand(ITexture* pTexture)
{
    TextureSharedPtr pTextureContext = static_cast<TextureImpl*>(pTexture)->getContext();

    MaterialSharedPtr pMaterial = m_pRetangleShape->getMaterial();

    if(pMaterial->getNumOfTextureUnitStates())
    {
        pMaterial->setTexture(0, pTextureContext);
    }
    else
    {
        TextureUnitState texUnit {pTextureContext};
        pMaterial->addTextureUnitState(texUnit);
    }

}

} // namespace glvr
} // namespace surveon





