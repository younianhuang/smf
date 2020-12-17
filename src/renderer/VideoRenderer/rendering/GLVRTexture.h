#ifndef GLVR_TEXTURE_H
#define GLVR_TEXTURE_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"
#include "GLVRImage.h"
#include "GLVRPixelBuffer.h"

namespace surveon
{
namespace glvr
{

class Texture
{
public:

    enum Format
    {
        PF_RED = GL_RED,
        PF_RGBA = GL_RGBA8,
        PF_BGRA = GL_BGRA
    };

public:
        Texture(void);
        ~Texture(void);

        void loadFromFile(const String& filename);

        void loadFromMemory(const uint8* data, size_t size);

        /** Create an empty texture */
        void create(void);

        /** Create an texture with specific format and dimension*/
        void create(Texture::Format format, uint32 width, uint32 height);

        /** Create an texture from external HardwarePixelBuffer*/
        void create(HardwarePixelBufferSharedPtr pPixelBuffer);

        HardwarePixelBufferSharedPtr getPixelBuffer(void);

        void blit(HardwarePixelBufferSharedPtr pSrc);

        void upload(const uint8* pixels, Texture::Format format, const mf::IntRect* pRect = nullptr);
        void bind(int32 textureUnit);
        void unbind(int32 textureUnit);

        uint32 getWidth(void) const
        {
            return m_Width;
        }

        uint32 getHeight(void) const
        {
            return m_Height;
        }

private:
        void loadFromImage(const Image& image);

private:
        GLuint m_TextureObjectId;
        Format m_Format;
        uint32 m_Width;
        uint32 m_Height;

        HardwarePixelBufferSharedPtr m_pPixelBuffer;
};

typedef std::shared_ptr<Texture> TextureSharedPtr;

} // namespace glvr
} // namespace surveon




#endif // GLVR_TEXTURE_H


