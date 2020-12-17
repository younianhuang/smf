
/*

*/
#ifndef GLVR_IRENDERABLE_H
#define GLVR_IRENDERABLE_H

#include "GLVRPrerequisites.h"
#include "GLVRColor.h"
#include <framework/SMFPrimitives.h>


namespace surveon
{

namespace glvr
{

/** Abstract base class for objects that can be rendered by VideoRenderer.
*/
class IRenderable
{
public:
        virtual ~IRenderable(void)    {}

        /** Gets the z order of this renderable object.*/
        virtual uint8 getZOrder(void) const = 0;

        /** Sets the z order of this renderable object.
         *    Z order is an ordering of overlapping. 0 is the lowest object which is rendered first,
         *    255 is the top most.
         *    In the same z order, Renderable object is rendered by the order of creation.
        */
        virtual void setZOrder(uint8 zorder) = 0;

        virtual void show(void) = 0;

        virtual void hide(void) = 0;

        virtual bool isShow(void) const = 0;

        virtual mf::FloatRect getBoundaryRect(void) const = 0;

        /**Set the global color of this renderable object.
         *   This color is modulated (multiplied) with the texture. It can be used to colorize the video, or change its global opacity.
         *   By default, is opaque white.
        */
        virtual void setColor(const Color& color) = 0;

        inline void setColor(float r, float g, float b, float a = 1.0f)
        {
            setColor(Color(r,g,b,a));
        }
};

} // namespace glvr
} // namespace surveon





#endif // GLVR_IRENDERABLE_H

