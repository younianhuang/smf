

#ifndef SMF_ANY_DETAIL_H
#define SMF_ANY_DETAIL_H


#include "framework/SMFPrerequisites.h"
#include "framework/SMFException.h"

namespace surveon
{
namespace mf
{
namespace detail
{

class PlaceHolder
{
public: // structors

    virtual ~PlaceHolder()
    {
    }

public: // queries

    virtual const std::type_info& getType() const  = 0;

    virtual PlaceHolder * clone() const = 0;

};


template<typename ValueType>
class Holder : public PlaceHolder
{
public: // structors

    Holder(const ValueType & value)
      : held(value)
    {
    }
/*
    Holder(ValueType&& value)
      : held(static_cast< ValueType&& >(value))
    {
    }
*/
public: // queries

    virtual const std::type_info& getType()  const
    {
        return typeid(ValueType);
    }

    virtual PlaceHolder * clone() const
    {
        return new Holder(held);
    }

public: // representation

    ValueType held;

private: // intentionally left unimplemented
    Holder & operator=(const Holder &);
};

} // private



} // namespace mf
} // namespace surveon



#endif // SMF_ANY_DETAIL_H






