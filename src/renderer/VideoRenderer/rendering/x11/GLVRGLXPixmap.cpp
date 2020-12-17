#include "GLVRGLXPixmap.h"

namespace surveon
{
namespace glvr
{

XPixmap::XPixmap(Display* display, int screen, Drawable drawable) :
    m_pDisplay(display),
    m_Screen(screen),
    m_Drawable(drawable),
    m_Pixmap(None),
    m_GC(None),
    m_GLXPixmap(None)
{
    assert(display);
    assert(drawable);
}

XPixmap::~XPixmap()
{
    destroy();
}


void* XPixmap::getNativeHandle(void)
{
    return reinterpret_cast<void*>(m_GLXPixmap);
}

void* XPixmap::getPrivateData(void)
{
    return reinterpret_cast<void*>(m_Pixmap);
}

void XPixmap::bindToTexture(uint texureId)
{    
    glXBindTexImageEXT(m_pDisplay, m_GLXPixmap, GLX_FRONT_EXT, NULL);
}

void XPixmap::unbindToTexture(void)
{
    glXReleaseTexImageEXT(m_pDisplay, m_GLXPixmap, GLX_FRONT_EXT);
}

uint32 XPixmap::getWidth(void) const
{
    return m_Width;
}

uint32 XPixmap::getHeight(void) const
{
    return m_Height;
}

mf::MediaSubType XPixmap::getFormat(void) const
{
    return SMF_VIDEO_FORMAT_XPIXMAP;
}

void XPixmap::blit(HardwarePixelBufferSharedPtr pSource)
{
    XPixmap* pSourcePixmap = dynamic_cast<XPixmap*>(pSource.get());
    assert(pSourcePixmap);

    XCopyArea(m_pDisplay, pSourcePixmap->m_Pixmap, m_Pixmap, m_GC,
              0, 0, m_Width, m_Height, 0, 0);
}


void XPixmap::create(uint width, uint height, uint depth)
{
    m_Width = width;
    m_Height = height;
    m_Depth = depth;

    assert(m_Width);
    assert(m_Height);
    assert(m_Depth);

    m_Pixmap = XCreatePixmap(m_pDisplay, m_Drawable, m_Width, m_Height, m_Depth);

    m_GC = XCreateGC(m_pDisplay, m_Pixmap, 0, NULL);

    int fbconfig_attrs[] = {
        GLX_DRAWABLE_TYPE,  GLX_PIXMAP_BIT,
        GLX_RENDER_TYPE,    GLX_RGBA_BIT,
        GLX_X_RENDERABLE,   GL_TRUE,
        GLX_Y_INVERTED_EXT, GL_TRUE,
        GLX_RED_SIZE,       8,
        GLX_GREEN_SIZE,     8,
        GLX_BLUE_SIZE,      8,
        GLX_ALPHA_SIZE,     8,
        GLX_DEPTH_SIZE,     16,
        GLX_BIND_TO_TEXTURE_RGBA_EXT,     GL_TRUE,
        GL_NONE
    };

    int nconfigs;
    GLXFBConfig *fbconfig = glXChooseFBConfig(m_pDisplay, m_Screen,
                                              fbconfig_attrs, &nconfigs);
    int pixmap_attrs[] = {
        GLX_TEXTURE_TARGET_EXT, GLX_TEXTURE_2D_EXT,
        GLX_MIPMAP_TEXTURE_EXT, GL_FALSE,
        GLX_TEXTURE_FORMAT_EXT, GLX_TEXTURE_FORMAT_RGB_EXT,
        GL_NONE
    };

    m_GLXPixmap = glXCreatePixmap(m_pDisplay, fbconfig[0], m_Pixmap,
                                                 pixmap_attrs);
    free(fbconfig);
}

void XPixmap::destroy(void)
{
    if (m_GLXPixmap != None)
    {
        glXDestroyGLXPixmap(m_pDisplay, m_GLXPixmap);
        m_GLXPixmap = None;
    }

    if(m_GC != None)
    {
        XFreeGC(m_pDisplay, m_GC);
        m_GC = None;
    }

    if (m_Pixmap != None)
    {
        XFreePixmap(m_pDisplay, m_Pixmap);
        m_Pixmap = None;
    }

}



} // namespace glvr
} // namespace surveon
