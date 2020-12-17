#include "GLVRTextureImpl.h"

namespace surveon
{
namespace glvr
{
    TextureImpl::TextureImpl(mf::AsyncCommandQueue* pCommandQueue) :
        m_pCommandQueue(pCommandQueue),
        m_pTexture(TextureManager::getInstance().createTexture())
    {

    }

    TextureImpl::~TextureImpl(void) {}

    void TextureImpl::loadFromFile(const String& filename)
    {
        m_pCommandQueue->blockingCall<void>(
                    AsyncCommand::CT_OPEN_FILE,
                    &Texture::loadFromFile, m_pTexture,
                    filename);
    }

    void TextureImpl::loadFromMemory(uint8* data, size_t size)
    {
        m_pCommandQueue->asyncCall<void>(
                    AsyncCommand::CT_OPEN_FILE,
                    &Texture::loadFromMemory, m_pTexture,
                    data, size);
    }

    void TextureImpl::create(uint32 width, uint32 height, ITexture::Format format)
    {
        m_pCommandQueue->blockingCall<void>(
                    AsyncCommand::CT_CREATE,
                    &TextureImpl::createCommand, this,
                    width, height, format);
    }

    void TextureImpl::upload(uint8* data, ITexture::Format format, const mf::IntRect* pRect)
    {
        m_pCommandQueue->blockingCall<void>(
                    AsyncCommand::CT_SET,
                    &Texture::upload, m_pTexture,
                    data, mapPixelFormat(format), pRect);
    }

    //====================================================================


    void TextureImpl::createCommand(uint32 width, uint32 height, ITexture::Format format)
    {
        m_pTexture->create(mapPixelFormat(format),
                           width, height);
    }

    Texture::Format TextureImpl::mapPixelFormat(ITexture::Format format)
    {
        switch(format)
        {
        case ITexture::PF_RED:
            return Texture::PF_RED;
        case ITexture::PF_RGBA:
            return Texture::PF_RGBA;
        case ITexture::PF_BGRA:
            return Texture::PF_BGRA;
        }

        return Texture::PF_RGBA;
    }


} // namespace glvr
} // namespace surveon

