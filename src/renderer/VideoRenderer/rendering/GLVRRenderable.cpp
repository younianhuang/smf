#include "GLVRRenderable.h"
#include "GLVRRenderTarget.h"

namespace surveon
{
namespace glvr
{

Renderable::Renderable(RenderTarget* pRenderTarget) :
    m_pRenderTarget(pRenderTarget),
    m_Show(true),
    m_ZOrder(0)
{
    assert(pRenderTarget);
    m_pRenderTarget->addRenderable(this);
}

Renderable::~Renderable()
{
    m_pRenderTarget->removeRenderable(this);
}

/*
void Renderable::show(void)
{
    m_Show = true;
}


void Renderable::hide(void)
{
    m_Show = false;
}
*/
void Renderable::setVisible(bool show)
{
    m_Show = show;
}


bool Renderable::isShow(void) const
{
    return m_Show;
}

uint8 Renderable::getZOrder(void) const
{
    return m_ZOrder;
}

void Renderable::setZOrder(uint8 zorder)
{
    m_ZOrder = zorder;
}

} // namespace glvr
} // namespace surveon



