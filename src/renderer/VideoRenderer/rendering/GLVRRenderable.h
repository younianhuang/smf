#ifndef GLVR_RENDERABLE_H
#define GLVR_RENDERABLE_H

#include "GLVRRenderOperation.h"
#include "GLVRMaterial.h"

namespace surveon
{
namespace glvr
{

class RenderTarget;

class Renderable
{
public:
    Renderable(RenderTarget* pRenderTarget);

    virtual ~Renderable(void);

    virtual void update(void) {}

    void show(void)
    {
        setVisible(true);
    }

    void hide(void)
    {
        setVisible(false);
    }

    virtual void setVisible(bool show);

    virtual bool isShow(void) const;

    virtual uint8 getZOrder(void) const;

    virtual void setZOrder(uint8 zorder);

    virtual void notifyViewportChanged(void) {};

    MaterialSharedPtr getMaterial(void)
    {
        return m_pMaterial;
    }

    RenderOperation& getRenderOperation(void)
    {
        return m_RenderOperation;
    }

    void setMaterial(MaterialSharedPtr pMaterial)
    {
        m_pMaterial = pMaterial;
    }   

protected:
    RenderTarget* m_pRenderTarget;

    RenderOperation m_RenderOperation;

    MaterialSharedPtr m_pMaterial;

    bool m_Show;

    uint8 m_ZOrder;
};

} // namespace glvr
} // namespace surveon

#endif // GLVR_RENDERABLE_H
