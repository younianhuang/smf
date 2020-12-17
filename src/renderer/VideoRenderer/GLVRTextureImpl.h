/*

*/
#ifndef GLVR_TEXTURE_IMPL_H
#define GLVR_TEXTURE_IMPL_H

#include "GLVRImplPrerequisites.h"
#include "GLVRITexture.h"

namespace surveon
{
namespace glvr
{

class TextureImpl : public ITexture
{
public:
    TextureImpl(mf::AsyncCommandQueue* pCommandQueue);

    virtual ~TextureImpl(void);

    virtual void loadFromFile(const String& filename);

    virtual void loadFromMemory(uint8* data, size_t size);

    virtual void create(uint32 width, uint32 height, ITexture::Format format = PF_RGBA);

    virtual void upload(uint8* data, ITexture::Format format = PF_RGBA, const mf::IntRect* pRect = nullptr);

    TextureSharedPtr getContext(void)
    {
        return m_pTexture;
    }

    static Texture::Format mapPixelFormat(ITexture::Format format);

private:
    void createCommand(uint32 width, uint32 height, ITexture::Format format);

private:
    mf::AsyncCommandQueue* m_pCommandQueue;

    TextureSharedPtr m_pTexture;
};

} // namespace glvr
} // namespace surveon


#endif // GLVR_TEXTURE_IMPL_H


