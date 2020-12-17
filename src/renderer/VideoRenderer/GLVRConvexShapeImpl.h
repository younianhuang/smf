/*

*/
#ifndef GLVR_CONVEX_SHAPE_IMPL_H
#define GLVR_CONVEX_SHAPE_IMPL_H

#include "GLVRImplPrerequisites.h"
#include "GLVRIConvexShape.h"

namespace surveon
{
namespace glvr
{

class ConvexShapeImpl : public IConvexShape
{
public:
    ConvexShapeImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue,
                    uint32 pointCount);

    virtual ~ConvexShapeImpl(void);

    virtual uint8 getZOrder(void) const;

    virtual void setZOrder(uint8 zorder);

    virtual void show(void);

    virtual void hide(void);

    virtual bool isShow(void) const;

    virtual void setColor(const Color& color);

    virtual void setOutlineColor(const Color& color);

    virtual void setOutlineThickness(int thickness);

    virtual void setNumOfPoints (uint32 count);

    virtual void setPoint (uint32 index, const Vector2f &point);

    virtual mf::FloatRect getBoundaryRect(void) const;

private:
    mf::AsyncCommandQueue* m_pCommandQueue;

    ConvexShape* m_pConvexShape;
};


} // namespace glvr
} // namespace surveon




#endif // GLVR_CONVEX_SHAPE_IMPL_H

