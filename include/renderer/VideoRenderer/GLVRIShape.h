/*

*/
#ifndef GLVR_ISHAPE_H
#define GLVR_ISHAPE_H

#include "GLVRPrerequisites.h"
#include "GLVRIRenderable.h"
//#include "GLVRTexture.h"
#include "GLVRColor.h"

namespace surveon
{

namespace glvr
{

/** Base class shapes with outline.
*/
class IShape : public IRenderable
{
public:

    virtual~IShape(void)  {}

    /** Sets the thickness of the shape's outline.
     * @param thickness New outline thickness.
     * Negative values are allowed (so that the outline expands towards the center of the shape),
     *  and using zero disables the outline.
     *  By default, the outline thickness is 0.
    */
    virtual void setOutlineThickness(int thickness) = 0;

    /** Sets the outline color of the shape.
     * @param color New outline color of the shape.
     * By default, the shape's outline color is opaque white.
    */
    virtual void setOutlineColor(const Color& color) = 0;       

    inline void setOutlineColor(float r, float g, float b, float a = 1.0f)
    {
        setOutlineColor(Color(r,g,b,a));
    }

};

} // namespace glvr
} // namespace surveon




#endif // GLVR_ISHAPE_H

