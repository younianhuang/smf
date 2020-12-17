#include "GLVRImage.h"
#include "stb_image/stb_image.h"

namespace surveon
{
namespace glvr
{

Image::Image() :
    m_Width(0),
    m_Height(0),
    m_Channels(0)
{

}

Image::~Image()
{

}

void Image::loadFromFile(const String& filename)
{
    // Load the image and get a pointer to the pixels in memory
   unsigned char* ptr = stbi_load(filename.c_str(), &m_Width, &m_Height, &m_Channels, STBI_rgb_alpha);

   if (ptr && m_Width && m_Height)
   {
       // Copy the loaded pixels to the pixel buffer
       m_pixels.resize(m_Width * m_Height * 4);
       memcpy(&m_pixels[0], ptr, m_pixels.size());

       // Free the loaded pixels (they are now in our own pixel buffer)
       stbi_image_free(ptr);
   }
   else
   {
       GLVR_THROW_EXCEPT(GLVRInternalErrorException, "Failed to load image " << filename << ": " << stbi_failure_reason())
   }
}

void Image::loadFromMemory(const uint8* data, size_t size)
{
    assert(data && size);
    // Load the image and get a pointer to the pixels in memory
    unsigned char* ptr = stbi_load_from_memory(data,  static_cast<int>(size), &m_Width, &m_Height, &m_Channels, STBI_rgb_alpha);

    if (ptr && m_Width && m_Height)
    {
       // Copy the loaded pixels to the pixel buffer
       m_pixels.resize(m_Width * m_Height * 4);
       memcpy(&m_pixels[0], ptr, m_pixels.size());

       // Free the loaded pixels (they are now in our own pixel buffer)
       stbi_image_free(ptr);
    }
    else
    {
       GLVR_THROW_EXCEPT(GLVRInternalErrorException, "Failed to load image from memory " )
    }
}

} // namespace glvr
} // namespace surveon



