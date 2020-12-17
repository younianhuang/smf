
#ifndef GLVR_COLOR_H
#define GLVR_COLOR_H

#include "GLVRPrerequisites.h"


namespace surveon
{
namespace glvr
{

class GLVR_API Color
{
public:
    static const Color Black;       ///< Black predefined color
    static const Color White;       ///< White predefined color
    static const Color Red;         ///< Red predefined color
    static const Color Green;       ///< Green predefined color
    static const Color Blue;        ///< Blue predefined color
    static const Color Yellow;      ///< Yellow predefined color
    static const Color Magenta;     ///< Magenta predefined color
    static const Color Cyan;        ///< Cyan predefined color
    static const Color Transparent; ///< Transparent (black) predefined color

public:
    Color() {}
    Color(float r_, float g_, float b_, float a_ = 1.0f) :
        r(r_), g(g_), b(b_), a(a_)
    { }

    ~Color()    {}

    float r {0.0f};
    float g {0.0f};
    float b {0.0f};
    float a {1.0f};
};

} // namespace glvr
} // namespace surveon



#endif // GLVR_COLOR_H
