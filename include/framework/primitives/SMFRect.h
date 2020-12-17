#ifndef SMF_RECT_H
#define SMF_RECT_H

#include "framework/SMFPrerequisites.h"

namespace surveon
{
namespace mf
{

/** Generic singleton class.
*/
template <class T>
class Rect
{
public:
    Rect();
    Rect(T l, T t, T r, T b);

    T getWidth(void) const
    {
        return right - left;
    }

    T getHeight(void) const
    {
        return bottom - top;
    }

    bool operator != (const Rect& rhs) const
    {
        return (left != rhs.left || top != rhs.top || right != rhs.right || bottom != rhs.bottom );
    }

    bool isInside(T x, T y) const
    {
         return x >= left && x <= right && y >= top && y <= bottom;
    }

    T left;
    T top;
    T right;
    T bottom;
};

template<class T>
Rect<T>::Rect() :
    left(T()),
    top(T()),
    right(T()),
    bottom(T())
{

}

template<class T>
Rect<T>::Rect(T l, T t, T r, T b) :
    left(l),
    top(t),
    right(r),
    bottom(b)
{

}

typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;

SMF_TEMPLATE_INSTANTIATE template class SMF_API Rect<int>;
SMF_TEMPLATE_INSTANTIATE template class SMF_API Rect<float>;

} // namespace mf
} // namespace surveon

#endif // SMF_RECT_H

