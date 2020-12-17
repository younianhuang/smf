#ifndef SMF_MAP_ITERATOR_H
#define SMF_MAP_ITERATOR_H

#include "framework/SMFPrerequisites.h"


namespace surveon
{
namespace mf
{

template<class MapType>
class MapIterator
{
public:
    typedef typename MapType::mapped_type ValueType;

    MapIterator(MapType& map)
    {
        m_Iterator = map.begin();
        m_End = map.end();
    }

    bool hasMore() const
    {
        return m_Iterator != m_End;
    }

    ValueType& getNext(void)
    {
        ValueType& value = m_Iterator->second;

        ++m_Iterator;

        return value;
    }

private:
    typename MapType::iterator m_End;
    typename MapType::iterator m_Iterator;
};

} // namespace mf
} // namespace surveon


#endif // SMF_MAP_ITERATOR_H

