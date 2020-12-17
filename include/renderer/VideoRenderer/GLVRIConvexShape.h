/*

*/
#ifndef SURVEON_GLVR_ICONVEX_SHAPE_H
#define SURVEON_GLVR_ICONVEX_SHAPE_H

#include "GLVRIShape.h"
#include "GLVRVector2.h"

namespace surveon
{

namespace glvr
{

/**  Class for representing a convex polygon.
 * It is important to keep in mind that a convex shape must always be convex, otherwise it may not be drawn correctly.
 * @remarks
 *      The points must be defined in order; using a random order would result in an incorrect shape.
 */
class IConvexShape : public IShape
{
public:

    virtual ~IConvexShape(void) {}

    /** Sets the number of points of the polygon
     * @remarks
     *  this function must be called before set point position./
    */
    virtual void setNumOfPoints (uint32 count) = 0;

    /** Sets the point position.
    */
    virtual void setPoint (uint32 index, const Vector2f &point) = 0;

    inline void setPoint (uint32 index, float x, float y)
    {
        setPoint(index, Vector2f(x, y));
    }
};

} // namespace glvr
} // namespace surveon




#endif // SURVEON_GLVR_ICONVEX_SHAPE_H

