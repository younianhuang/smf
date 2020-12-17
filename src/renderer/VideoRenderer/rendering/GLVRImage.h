
#ifndef GLVR_IMAGE_H
#define GLVR_IMAGE_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"

namespace surveon
{
namespace glvr
{

class Image
{
public:
    Image();
    ~Image();

    void loadFromFile(const String& filename);

    void loadFromMemory(const uint8* data, size_t size);

    uint32 getWidth(void) const
    {
        return m_Width;
    }

    uint32 getHeight(void) const
    {
        return m_Height;
    }

    const uint8* getPixels(void) const
    {
        return m_pixels.data();
    }

private:
    std::vector<uint8> m_pixels;
    int m_Width;
    int m_Height;
    int m_Channels;
};


} // namespace glvr
} // namespace surveon



#endif // GLVR_IMAGE_H

