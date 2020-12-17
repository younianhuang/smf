/*

*/

#ifndef GLVR_ITEXTURE_H
#define GLVR_ITEXTURE_H

#include "GLVRPrerequisites.h"


namespace surveon
{
namespace glvr
{

/** Class for representing an image living on the graphics card that can be used for rendering.
 *  Texture can be shared by different VideoRenderer.
*/
class ITexture
{
public:
    enum Format
    {
        PF_RED = 0,
        PF_RGBA,
        PF_BGRA
    };

public:        
        virtual ~ITexture(void) {}

        /** Load the texture from a file on disk.
         * The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic.
        */
        virtual void loadFromFile(const String& filename) = 0;

        /** Load the texture from a file in memory.
         * The supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic.
        */
        virtual void loadFromMemory(uint8* data, size_t size) = 0;

        /** Create the texture manually.
         * @param width width of this texture.
         * @param height height of this texture.
         * @param format pixel format of this texture.
        */
        virtual void create(uint32 width, uint32 height, Format format = PF_RGBA) = 0;

        /** Update the texture from an array of pixels.
         * @param pixels the pixel data.
         * @param format the format of pixel data.
         * @param pRect the sub area of texture to be update. The whole texutre if null.
        */
        virtual void upload(uint8* pixels, Format format = PF_RGBA, const mf::IntRect* pRect = nullptr) = 0;
};


} // namespace glvr
} // namespace surveon




#endif // GLVR_ITEXTURE_H

