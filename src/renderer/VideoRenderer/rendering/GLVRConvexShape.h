#ifndef GLVR_CONVEX_SHAPE_H
#define GLVR_CONVEX_SHAPE_H

#include "GLVRRenderable.h"
#include "GLVRRenderTarget.h"
#include "GLVRVector2.h"
#include "GLVRColor.h"
#include "GLVRShape.h"

namespace surveon
{
namespace glvr
{

class ConvexShape : public Shape
{
public:
    ConvexShape(RenderTarget* pRenderTarget, uint32 pointCount);

    virtual ~ConvexShape();


protected:
    virtual void createHardwareBuffers(void);

    virtual void updateGeometry(void);

protected:

};

} // namespace glvr
} // namespace surveon


#endif // GLVR_CONVEX_SHAPE_H


