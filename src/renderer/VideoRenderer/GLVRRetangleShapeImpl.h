/*

*/
#ifndef GLVR_RETANGLE_SHAPE_IMPL_H
#define GLVR_RETANGLE_SHAPE_IMPL_H

#include "GLVRImplPrerequisites.h"
#include "GLVRIRetangleShape.h"

namespace surveon
{
namespace glvr
{

class RetangleShapeImpl : public IRetangleShape
{
public:
    RetangleShapeImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue, const mf::FloatRect& rect);

    virtual ~RetangleShapeImpl(void);

    virtual uint8 getZOrder(void) const;

    virtual void setZOrder(uint8 zorder);

    virtual void show(void);

    virtual void hide(void);

    virtual bool isShow(void) const;

    virtual void setColor(const Color& color);

    virtual void setOutlineColor(const Color& color);

    virtual void setOutlineThickness(int thickness);

    virtual void setRect(const mf::FloatRect& rect);

    virtual mf::FloatRect getRect(void) const;

    virtual mf::FloatRect getBoundaryRect(void) const;

private:
    mf::AsyncCommandQueue* m_pCommandQueue;

    RetangleShape* m_pRetangleShape;
};


} // namespace glvr
} // namespace surveon




#endif // GLVR_RETANGLE_SHAPE_IMPL_H

