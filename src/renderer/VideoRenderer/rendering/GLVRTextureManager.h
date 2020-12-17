
#ifndef GLVR_TEXTURE_MANAGER_H
#define GLVR_TEXTURE_MANAGER_H

#include "GLVRTexture.h"


namespace surveon
{
namespace glvr
{

class TextureManager : public Singleton<TextureManager>
{
    friend class Singleton<TextureManager>;

public:

    TextureSharedPtr createTexture(void);

    TextureSharedPtr createTexture(Texture::Format format, int width, int height);

private:
    TextureManager();
    ~TextureManager();
};

} // namespace glvr
} // namespace surveon




#endif // GLVR_TEXTURE_MANAGER_H

