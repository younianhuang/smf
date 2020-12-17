
#ifndef GLVR_VIEWPORT_H
#define GLVR_VIEWPORT_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRVector3.h"

namespace surveon
{
namespace glvr
{

class Viewport
{
public:
    Viewport()  {}
    Viewport(mf::FloatRect& rect);
    ~Viewport() {}

    float getLeft(void) const
    {
        return m_Rect.left;
    }

    float getTop(void) const
    {
        return m_Rect.top;
    }

    float getRight(void) const
    {
        return m_Rect.right;
    }

    float getBottom(void) const
    {
        return m_Rect.bottom;
    }

    float getWidth(void) const
    {
        return m_Width;
    }

    float getHeight(void) const
    {
        return m_Height;
    }

    void setRect(const mf::FloatRect& rect)
    {
        m_Rect = rect;
        m_Width = rect.getWidth();
        m_Height = rect.getHeight();
    }

    const mf::FloatRect& getRect(void) const
    {
        return m_Rect;
    }

    float screenXCoordToNDC(float sx) const
    {
        return 2.0f * sx/m_Width - 1.0f;
    }

    float screenYCoordToNDC(float sy) const
    {
        return 1.0f -2.0f * sy/m_Height;
    }

    /** Convert screen coordinate to Normalized Decive Coordinate.
     */
    mf::FloatRect screenCoordToNDC(const mf::FloatRect& screenRect) const;

    Vector3f screenCoordToNDC(float sx, float sy) const
    {
        return Vector3f(
                        screenXCoordToNDC(sx),
                        screenYCoordToNDC(sy) );
    }

    Vector3f screenCoordToNDC(float sx, float sy, float z) const
    {
        return Vector3f(
                        screenXCoordToNDC(sx),
                        screenYCoordToNDC(sy),
                        z );
    }

private:
    mf::FloatRect m_Rect;
    float m_Width {0.0f};
    float m_Height {0.0f};
};

} // namespace glvr
} // namespace surveon


#endif // GLVR_VIEWPORT_H
