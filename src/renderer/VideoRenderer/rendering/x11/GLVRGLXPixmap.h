
#ifndef GLVR_GLXPIXMAP_H
#define GLVR_GLXPIXMAP_H


#include "GLVRX11Prerequisites.h"
#include "../GLVRPixelBuffer.h"


namespace surveon
{
namespace glvr
{

class XPixmap : public HardwarePixelBuffer
{
public:
    XPixmap(Display* display, int screen, Drawable drawable);

    virtual ~XPixmap();

    virtual void* getNativeHandle(void);

    virtual void* getPrivateData(void);

    virtual void bindToTexture(uint texureId);

    virtual void unbindToTexture(void);

    virtual uint32 getWidth(void) const;

    virtual uint32 getHeight(void) const;

    virtual mf::MediaSubType getFormat(void) const;

    virtual void blit(HardwarePixelBufferSharedPtr pSource);

    void create(uint width, uint height, uint depth);

    void destroy(void);

private:

    Display* m_pDisplay;

    int m_Screen;

    Drawable m_Drawable;

    Pixmap m_Pixmap;

    GC m_GC;

    GLXPixmap m_GLXPixmap;

    uint m_Width;

    uint m_Height;

    uint m_Depth;
};



} // namespace glvr
} // namespace surveon

#endif // GLVR_GLXPIXMAP_H



