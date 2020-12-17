#include "GLVRTextureManager.h"


namespace surveon
{
namespace glvr
{

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

}

TextureSharedPtr TextureManager::createTexture(void)
{
    return TextureSharedPtr(new Texture());
}

TextureSharedPtr TextureManager::createTexture(Texture::Format format, int width, int height)
{
    TextureSharedPtr pTexture(new Texture());

    pTexture->create(format, width, height);

    return pTexture;
}

} // namespace glvr
} // namespace surveon



