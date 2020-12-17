#ifndef GLVR_VECTOR2_H
#define GLVR_VECTOR2_H

#include "GLVRPrerequisites.h"

namespace surveon
{
namespace glvr
{

template<class T>
class Vector2
{
public:
    Vector2(void) {}

    Vector2(const T x_, const T y_) :
            x(x_), y(y_)
    {}

    float getLength(void) const
    {
        return std::sqrt(static_cast<float>(x * x + y * y));
    }

    /** Normalize this vector*/
    void normalize(void)
    {
        float length = getLength();
        if(length > 0)
        {
            x = static_cast<T>(x/length);
            y = static_cast<T>(y/length);
        }
    }

    T x {T()};
    T y {T()};
};


template<class T>
Vector2<T> operator- (const Vector2<T>& lhs ,const Vector2<T>& rhs)
{
    return Vector2<T> (lhs.x - rhs.x, lhs.y - rhs.y);
}

template<class T>
Vector2<T> operator+ (const Vector2<T>& lhs ,const Vector2<T>& rhs)
{
    return Vector2<T> (lhs.x + rhs.x, lhs.y + rhs.y);
}

template<class T>
T operator* (const Vector2<T>& lhs , const Vector2<T>& rhs)
{
    return  (lhs.x * rhs.x + lhs.y * rhs.y);
}

template<class T, class S>
Vector2<T> operator* (const Vector2<T>& lhs , S rhs)
{
    return Vector2<T> (lhs.x * rhs, lhs.y * rhs);
}

template<class T, class S>
Vector2<T> operator/ (const Vector2<T>& lhs , S rhs)
{
    return Vector2<T> (lhs.x / rhs, lhs.y / rhs);
}


typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

GLVR_TEMPLATE_INSTANTIATE template class Vector2<int>;
GLVR_TEMPLATE_INSTANTIATE template class Vector2<float>;


} // namespace glvr
} // namespace surveon


#endif // GLVR_VECTOR2_H
