/*

*/
#ifndef GLVR_IRETANGLE_SHAPE_H
#define GLVR_IRETANGLE_SHAPE_H

#include "GLVRPrerequisites.h"
#include "GLVRIShape.h"

namespace surveon
{
namespace glvr
{

/** Class for representing a rectangle shape.
*/
class IRetangleShape : public IShape
{
public:

    virtual ~IRetangleShape(void) {}

    virtual void setRect(const mf::FloatRect& rect) = 0;

    virtual mf::FloatRect getRect(void) const = 0;

    inline void setRect(float left, float top, float right, float bottom)
    {
        setRect(FloatRect(left, top, right, bottom));
    }

};

} // namespace glvr
} // namespace surveon




#endif // GLVR_IRETANGLE_SHAPE_H

