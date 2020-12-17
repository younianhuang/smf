
#include "GLVRConvexShapeImpl.h"

namespace surveon
{

namespace glvr
{


ConvexShapeImpl::ConvexShapeImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue,
                                 uint32 pointCount) :
    m_pCommandQueue(pCommandQueue),
    m_pConvexShape(new ConvexShape(pRenderTarget, pointCount))
{
    GLVR_LOG_INFO( "ConvexShape "  << SMF_ADDR_PREFIX(this) << " is created");

    MaterialSharedPtr pMaterial(new Material);

    pMaterial->setProgram(GPUProgramManager::getInstance().getDefaultGPUProgram());

    m_pConvexShape->setMaterial(pMaterial);
}

ConvexShapeImpl::~ConvexShapeImpl()
{    
    delete m_pConvexShape;

    GLVR_LOG_INFO( "ConvexShape "  << SMF_ADDR_PREFIX(this) << " is destroyed");
}

uint8 ConvexShapeImpl::getZOrder(void) const
{
    return m_pCommandQueue->blockingCall<uint8>(
                AsyncCommand::CT_GET,
                &ConvexShape::getZOrder, m_pConvexShape);
}

void ConvexShapeImpl::setZOrder(uint8 zorder)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::setZOrder, m_pConvexShape, zorder);
}

void ConvexShapeImpl::show(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::show, m_pConvexShape);
}

void ConvexShapeImpl::hide(void)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::hide, m_pConvexShape);
}

bool ConvexShapeImpl::isShow(void) const
{
    return m_pCommandQueue->blockingCall<bool>(
                AsyncCommand::CT_GET,
                &ConvexShape::isShow, m_pConvexShape);
}

void ConvexShapeImpl::setColor(const Color& color)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::setFillColor, m_pConvexShape,
                color);
}

void ConvexShapeImpl::setOutlineColor(const Color& color)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::setOutlineColor, m_pConvexShape,
                color);
}

void ConvexShapeImpl::setOutlineThickness(int thickness)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::setOutlineThickness, m_pConvexShape,
                thickness);
}

void ConvexShapeImpl::setNumOfPoints (uint32 count)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::setPointCount, m_pConvexShape,
                count);
}

void ConvexShapeImpl::setPoint (uint32 index, const Vector2f &point)
{
    m_pCommandQueue->asyncCall<void>(
                AsyncCommand::CT_SET,
                &ConvexShape::setPoint, m_pConvexShape,
                index, point);
}

mf::FloatRect ConvexShapeImpl::getBoundaryRect(void) const
{
    return m_pCommandQueue->blockingCall<mf::FloatRect>(
                AsyncCommand::CT_GET,
                &ConvexShape::getBoundaryRect, m_pConvexShape);
}

} // namespace glvr
} // namespace surveon





