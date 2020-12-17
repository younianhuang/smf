
#ifndef SMF_SEQUENCE_CONTAINER_ITERATOR_H
#define SMF_SEQUENCE_CONTAINER_ITERATOR_H

#include "framework/SMFPrerequisites.h"


namespace surveon
{
namespace mf
{

template<class ContainerType>
class SequenceContainerIterator
{
public:
    typedef typename ContainerType::value_type ValueType;

    SequenceContainerIterator(ContainerType& container)
    {
        m_Iterator = container.begin();
        m_End = container.end();
    }

    bool hasMore() const
    {
        return m_Iterator != m_End;
    }

    const ValueType& getNext(void)
    {
        return *m_Iterator++;
    }


private:
    typename ContainerType::iterator m_End;
    typename ContainerType::iterator m_Iterator;
};

} // namespace mf
} // namespace surveon


#endif // SMF_SEQUENCE_CONTAINER_ITERATOR_H




