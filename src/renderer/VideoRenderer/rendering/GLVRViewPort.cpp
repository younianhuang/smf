#include "GLVRViewPort.h"


namespace surveon
{
namespace glvr
{

Viewport::Viewport(mf::FloatRect& rect) :
    m_Rect(rect),
    m_Width(rect.getWidth()),
    m_Height(rect.getHeight())
{

}

mf::FloatRect Viewport::screenCoordToNDC(const mf::FloatRect& screenRect) const
{
    return mf::FloatRect(
                    screenXCoordToNDC(screenRect.left),
                    screenYCoordToNDC(screenRect.top),
                    screenXCoordToNDC(screenRect.right),
                    screenYCoordToNDC(screenRect.bottom)
                );
}



} // namespace glvr
} // namespace surveon
