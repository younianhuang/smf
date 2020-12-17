#ifndef SMF_SIZE_H
#define SMF_SIZE_H


#include "framework/SMFPrerequisites.h"

namespace surveon
{
namespace mf
{

struct Size
{
    int width;
    int height;

    Size() : width(0), height(0)    {}
    Size(int w, int h) : width(w), height(h)   {}
};


} // namespace mf
} // namespace surveon



#endif // SMF_SIZE_H



