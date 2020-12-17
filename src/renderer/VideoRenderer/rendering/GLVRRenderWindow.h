#ifndef GLVR_RENDER_WINDOW_H
#define GLVR_RENDER_WINDOW_H

#include "GLVRRenderTarget.h"
#include "GLVRRenderable.h"
#include "GLVRColor.h"
#include "GLVRPixelBuffer.h"

namespace surveon
{
namespace glvr
{

class RenderWindow : public RenderTarget
{
public:
    RenderWindow();

    virtual ~RenderWindow();

    virtual void open(uint32 width, uint32 height, const String& title);

    virtual void open(mf::WindowHandle handle);

    virtual void close(void) = 0;

    virtual void swapBuffer(void) = 0;

    virtual void makeCurrent(bool active) = 0;

    virtual bool isOpen(void) const;

    virtual void clearFrameBuffer(const Color& color);    

    bool isPrimary(void) const
    {
        return m_Priamry;
    }

    void setPrimary(bool primary)
    {
        m_Priamry = primary;
    }

    void resetGLState(void);

    //virtual void hide(void);

protected:
    void releaseResources(void);

protected:
/*
    mf::WindowHandle m_WindowHandle {mf::WindowHandle()};

    sf::RenderWindow m_SFMLWindow;
*/
    RenderStates m_RenderStates;

    bool m_Priamry;

    bool m_Open;

    bool m_OwnWindow;
};

} // namespace glvr
} // namespace surveon

#endif // GLVR_RENDER_WINDOW_H
