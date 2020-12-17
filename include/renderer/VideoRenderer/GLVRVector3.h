#ifndef GLVR_VECTOR3_H
#define GLVR_VECTOR3_H

#include "GLVRPrerequisites.h"

namespace surveon
{
namespace glvr
{

template<class T>
class Vector3
{
public:
    Vector3(void) {}

    Vector3(const T x_, const T y_, const T z_ = 1) :
            x(x_), y(y_), z(z_)
    {}

    T x {T()};
    T y {T()};
    T z {T()};
};

typedef Vector3<float> Vector3f;

GLVR_TEMPLATE_INSTANTIATE template class Vector3<float>;

} // namespace glvr
} // namespace surveon


#endif // GLVR_VECTOR3_H
