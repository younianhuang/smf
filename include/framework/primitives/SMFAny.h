

#ifndef SMF_Any_H
#define SMF_Any_H


#include "framework/SMFPrerequisites.h"
#include "framework/SMFException.h"
#include "framework/primitives/SMFAnyHolder.h"

namespace surveon
{
namespace mf
{
class Any
{
public:
    Any() : m_pContent(nullptr)
    {
    }

    template<typename ValueType>
    Any(const ValueType & value)  : m_pContent(new detail::Holder<ValueType>(value))
    {
    }

    Any(const Any & other) : m_pContent(other.m_pContent ? other.m_pContent->clone() : nullptr)
    {
    }

    // Move constructor
    Any(Any&& other)  :
        m_pContent(other.m_pContent)
    {
        other.m_pContent = 0;
    }
/*
    // Perfect forwarding of ValueType
    template<typename ValueType>
    Any(ValueType&& value) :
        m_pContent(new Holder<ValueType>(static_cast<ValueType&&>(value)))
    {
    }
*/
    ~Any()
    {
        delete m_pContent;
    }

public: // modifiers

    Any & swap(Any & rhs)
    {
        std::swap(m_pContent, rhs.m_pContent);
        return *this;
    }

    Any & operator=(const Any& rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    // move assignement
    Any & operator=(Any&& rhs)
    {
        rhs.swap(*this);
        Any().swap(rhs);
        return *this;
    }

    // Perfect forwarding of ValueType
/*
    template <class ValueType>
    Any & operator=(ValueType&& rhs)
    {
        Any(static_cast<ValueType&&>(rhs)).swap(*this);

        return *this;
    }
*/

    template <class ValueType>
    Any & operator=(const ValueType& rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    template <class ValueType>
    const ValueType& getContentAs(void) const
    {
        if(getType() != typeid(ValueType))
        {
            SMF_THROW_EXCEPT(InvalidTypeException, "the content of this object is not type: "
                             << typeid(ValueType).name());
        }

        return static_cast<detail::Holder<ValueType>*>(m_pContent)->held;
    }

public: // queries

    bool isEmpty() const
    {
        return !m_pContent;
    }

    void clear()
    {
        Any().swap(*this);
    }

    const std::type_info& getType() const
    {
        return m_pContent ? m_pContent->getType() : typeid(void);
    }

    inline void swap(Any & lhs, Any & rhs)
    {
        lhs.swap(rhs);
    }
private:
        detail::PlaceHolder* m_pContent;
};

} // namespace mf
} // namespace surveon



#endif // SMF_Any_H





