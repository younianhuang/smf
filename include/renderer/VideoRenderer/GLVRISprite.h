/*

*/
#ifndef GLVR_ISPRITE_H
#define GLVR_ISPRITE_H

#include "GLVRPrerequisites.h"
#include "GLVRIRenderable.h"
#include "GLVRITexture.h"

namespace surveon
{
namespace glvr
{
/** Class for representing a image.
*/
class ISprite : public IRenderable
{
public:

    virtual ~ISprite(void) {}

    virtual void setRect(const mf::FloatRect& rect) = 0;

    virtual mf::FloatRect getRect(void) const = 0;

    inline void setRect(float left, float top, float right, float bottom)
    {
        setRect(FloatRect(left, top, right, bottom));
    }

    virtual void setBorderColor(const Color& color) = 0;

    virtual void setBorderThickness(int thickness) = 0;

    virtual void setTexture(ITexture* pTexture) = 0;

    virtual void setTextureRect(const mf::FloatRect& rect) = 0;
};

} // namespace glvr
} // namespace surveon




#endif // GLVR_IRETANGLE_SHAPE_H



