#ifndef SMF_ATTRIBUTES_H
#define SMF_ATTRIBUTES_H

#include "framework/SMFPrerequisites.h"
#include "framework/primitives/SMFAny.h"
#include "framework/thread/SMFMutex.h"


namespace surveon
{
namespace mf
{

typedef String AttributeId;



class Attributes
{
public:

    typedef std::function<void(const AttributeId& id)> Callback;

public:
    Attributes(void)    {}
    ~Attributes(void)    {}

    template<class ValueType>
    void setAttribuate(const AttributeId& id, const ValueType& value)
    {
        SMF_MUTEX_LOCK(m_Mutex);

        auto it = m_AttributeMap.find(id);
        if(it == m_AttributeMap.end())
        {
            m_AttributeMap.insert(AttributeMap::value_type(id, Any(value)));
        }
        else
        {
            Any& attribute = it->second;
            if(attribute.getType() != typeid(ValueType))
            {
                SMF_THROW_EXCEPT(InvalidTypeException,
                                 "Attribute " << id << " is not type: " << typeid(ValueType).name() );
            }

            attribute = value;
        }

        if(m_AttributeChangedCallback)
            m_AttributeChangedCallback(id);
    }

    template<class ValueType>
    const ValueType& getAttribuate(const AttributeId& id) const
    {
        SMF_MUTEX_LOCK(m_Mutex);

        auto it = m_AttributeMap.find(id);
        if(it == m_AttributeMap.end())
        {
            SMF_THROW_EXCEPT(InvalidParametersException, "Attribute " << id << " does not exist");
        }

        return it->second.getContentAs<ValueType>();
    }

    bool hasAttribute(const AttributeId& id) const
    {
        SMF_MUTEX_LOCK(m_Mutex);

        return m_AttributeMap.count(id) >0;
    }

    template<class Fn, class Object>
    void setAttributeChangedCallback(Fn&& fn, Object* pObj)
    {
        SMF_MUTEX_LOCK(m_Mutex);

        m_AttributeChangedCallback = std::bind(fn, pObj, std::placeholders::_1);
    }


private:
    typedef std::map<AttributeId, Any> AttributeMap;
    AttributeMap m_AttributeMap;

    Callback m_AttributeChangedCallback;

    mutable Mutex m_Mutex;
};



} // namespace mf
} // namespace surveon




#endif // SMF_ATTRIBUTES_H

