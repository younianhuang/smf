#ifndef GLVR_RETANGLE_SHAPE_H
#define GLVR_RETANGLE_SHAPE_H

#include "GLVRRenderable.h"
#include "GLVRRenderTarget.h"
#include "GLVRVector2.h"
#include "GLVRColor.h"
#include "GLVRShape.h"

namespace surveon
{
namespace glvr
{

class RetangleShape : public Shape
{
public:
    RetangleShape(RenderTarget* pRenderTarget, const mf::FloatRect& rect);

    virtual ~RetangleShape();

    void setRect(const mf::FloatRect& rect);

    const mf::FloatRect& getRect(void) const
    {
        return m_Rect;
    }

protected:
    void createHardwareBuffers(void);

    virtual void updateGeometry(void);

    virtual void updateTextureGeometry(void);    

protected:    
    FloatRect m_Rect;

};

} // namespace glvr
} // namespace surveon


#endif // GLVR_RETANGLE_SHAPE_H


