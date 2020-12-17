/*

*/
#ifndef GLVR_SPRITE_IMPL_H
#define GLVR_SPRITE_IMPL_H

#include "GLVRImplPrerequisites.h"
#include "GLVRISprite.h"


namespace surveon
{
namespace glvr
{

class SpriteImpl : public ISprite
{
public:
   SpriteImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue, const mf::FloatRect& rect);

    virtual ~SpriteImpl(void);

    virtual uint8 getZOrder(void) const;

    virtual void setZOrder(uint8 zorder);

    virtual void show(void);

    virtual void hide(void);

    virtual bool isShow(void) const;

    virtual void setColor(const Color& color);

    virtual void setBorderColor(const Color& color);

    virtual void setBorderThickness(int thickness);

    virtual void setRect(const mf::FloatRect& rect);

    virtual mf::FloatRect getRect(void) const;

    virtual void setTexture(ITexture* pTexture);

    virtual void setTextureRect(const mf::FloatRect& rect);

    virtual mf::FloatRect getBoundaryRect(void) const;

private:
    void setTextureCommand(ITexture* pTexture);

private:
    mf::AsyncCommandQueue* m_pCommandQueue;

    RetangleShape* m_pRetangleShape;
};


} // namespace glvr
} // namespace surveon




#endif // GLVR_SPRITE_IMPL_H

