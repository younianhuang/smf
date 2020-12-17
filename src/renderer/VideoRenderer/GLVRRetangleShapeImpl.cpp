
#include "GLVRRetangleShapeImpl.h"

namespace surveon
{

namespace glvr
{


RetangleShapeImpl::RetangleShapeImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue, const mf::FloatRect& rect) :
    m_pCommandQueue(pCommandQueue),
    m_pRetangleShape(new RetangleShape(pRenderTarget, rect))
{
    GLVR_LOG_INFO( "RetangleShape "  << SMF_ADDR_PREFIX(this) << " is created");

    MaterialSharedPtr pMaterial(new Material);

    pMaterial->setProgram(GPUProgramManager::getInstance().getDefaultGPUProgram());

    m_pRetangleShape->setMaterial(pMaterial);
}

RetangleShapeImpl::~RetangleShapeImpl()
{    
    delete m_pRetangleShape;

    GLVR_LOG_INFO( "RetangleShape "  << SMF_ADDR_PREFIX(this) << " is destroyed");
}

uint8 RetangleShapeImpl::getZOrder(void) const
{
    return m_pCommandQueue->blockingCall<uint8>(
                AsyncCommand::CT_GET,
                &RetangleShape::getZOrder, m_pRetangleShape);
}

void RetangleShapeImpl::setZOrder(uint8 zorder)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setZOrder, m_pRetangleShape,
                zorder);
}

void RetangleShapeImpl::show(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::show, m_pRetangleShape);
}

void RetangleShapeImpl::hide(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::hide, m_pRetangleShape);
}

bool RetangleShapeImpl::isShow(void) const
{
    return m_pCommandQueue->blockingCall<bool>(
                AsyncCommand::CT_GET,
                &RetangleShape::isShow, m_pRetangleShape);
}

void RetangleShapeImpl::setColor(const Color& color)
{
    return m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setFillColor, m_pRetangleShape,
                color);
}

void RetangleShapeImpl::setOutlineColor(const Color& color)
{
    return m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setOutlineColor, m_pRetangleShape,
                color);
}

void RetangleShapeImpl::setOutlineThickness(int thickness)
{
    return m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setOutlineThickness, m_pRetangleShape,
                thickness);
}

void RetangleShapeImpl::setRect(const mf::FloatRect& rect)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &RetangleShape::setRect, m_pRetangleShape,
                rect);
}

mf::FloatRect RetangleShapeImpl::getRect(void) const
{
    return m_pCommandQueue->blockingCall<mf::FloatRect>(
                AsyncCommand::CT_GET,
                &RetangleShape::getRect, m_pRetangleShape);
}

mf::FloatRect RetangleShapeImpl::getBoundaryRect(void) const
{
    return m_pCommandQueue->blockingCall<mf::FloatRect>(
                AsyncCommand::CT_GET,
                &RetangleShape::getBoundaryRect, m_pRetangleShape);
}

} // namespace glvr
} // namespace surveon





