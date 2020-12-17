#include "GLVRTexture.h"

namespace surveon
{
namespace glvr
{

    Texture::Texture(void) :
        m_TextureObjectId(0),
        m_Format(PF_RGBA),
        m_Width(0),
        m_Height(0)

    {

    }

    Texture::~Texture(void)
    {
        if(m_TextureObjectId)
        {
            GLVR_GL_CHECK(glDeleteTextures(1, &m_TextureObjectId));
        }
    }

    void Texture::loadFromFile(const String& filename)
    {
        Image image;

        image.loadFromFile(filename);

        loadFromImage(image);
    }

    void Texture::loadFromMemory(const uint8* data, size_t size)
    {
        Image image;

        image.loadFromMemory(data, size);

        loadFromImage(image);
    }

    void Texture::loadFromImage(const Image& image)
    {
        m_Width = image.getWidth();
        m_Height = image.getHeight();
        m_Format = PF_RGBA;

        GLVR_GL_CHECK(glGenTextures(1, &m_TextureObjectId));
        GLVR_GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_TextureObjectId));

        GLVR_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0,
                                        GL_RGBA, GL_UNSIGNED_BYTE, image.getPixels()));
        GLVR_GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLVR_GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    void Texture::create(Texture::Format format, uint32 width, uint32 height)
    {
        m_Width = width;
        m_Height = height;
        m_Format = format;

        GLVR_GL_CHECK(glGenTextures(1, &m_TextureObjectId));
        GLVR_GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_TextureObjectId));

        GLVR_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0,
                                        GL_RED, GL_UNSIGNED_BYTE, NULL));
        GLVR_GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLVR_GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    void Texture::create(void)
    {
        GLVR_GL_CHECK(glGenTextures(1, &m_TextureObjectId));
        GLVR_GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_TextureObjectId));

        GLVR_GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLVR_GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    void Texture::create(HardwarePixelBufferSharedPtr pPixelBuffer)
    {
        create();

        m_pPixelBuffer = pPixelBuffer;
    }

    HardwarePixelBufferSharedPtr Texture::getPixelBuffer(void)
    {
        return m_pPixelBuffer;
    }

    void Texture::blit(HardwarePixelBufferSharedPtr pSrc)
    {
        assert(m_pPixelBuffer);
        m_pPixelBuffer->unbindToTexture();

        m_pPixelBuffer->blit(pSrc);

        m_pPixelBuffer->bindToTexture(m_TextureObjectId);
    }

    void Texture::upload(const uint8* pixels, Texture::Format format, const mf::IntRect* pRect)
    {
        if(!pixels)
        {
            GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Invalid null pixels data pointer!");
        }

        if( pRect &&
            (static_cast<uint32>(pRect->right) > m_Width || static_cast<uint32>(pRect->bottom) > m_Height) )
        {
            GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Invalid offset or size.");
        }

        GLenum pixFormat =  GL_RED;

        switch(format)
        {
            case PF_RED:
                pixFormat = GL_RED;                
                break;
            case  PF_RGBA:
                pixFormat = GL_RGBA;
            break;
        case  PF_BGRA:
            pixFormat = GL_BGRA;
        break;
        }

        GLenum componentType = GL_UNSIGNED_BYTE;

        GLVR_GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_TextureObjectId));

        if(pRect)
        {
            GLVR_GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0,
                                          pRect->left, pRect->top, pRect->getWidth(), pRect->getHeight(),
                                          pixFormat, componentType,
                                          pixels));
        }
        else
        {            
            GLVR_GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0,
                                          0, 0, m_Width, m_Height,
                                          pixFormat, componentType,
                                          pixels));
        }
    }

    void Texture::bind(int32 textureUnit)
    {
        GLVR_GL_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));
        GLVR_GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_TextureObjectId));

        if(m_pPixelBuffer)
            m_pPixelBuffer->bindToTexture(m_TextureObjectId);
    }

    void Texture::unbind(int32 textureUnit)
    {
        if(m_pPixelBuffer)
            m_pPixelBuffer->unbindToTexture();

        GLVR_GL_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));
        GLVR_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));                
    }

} // namespace glvr
} // namespace surveon
