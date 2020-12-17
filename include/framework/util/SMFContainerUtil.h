
#ifndef SMF_CONTAINER_UTIL_H
#define SMF_CONTAINER_UTIL_H

namespace surveon
{
namespace mf
{

    class ContainerUtil
    {
    public:
        template <class T>
        static void destroyAllElements(T& container)
        {
            for(auto pElement : container)
            {
                delete pElement;
            }

            container.clear();
        }

        template <class T>
        static void destroyMapAllSeconds(T& container)
        {
            for(auto& it : container)
            {
                delete it.second;
            }

            container.clear();
        }

        template<class Set, class Element>
        static bool destroySetElement(Set& set, Element *pElem)
        {
            if(set.erase(pElem))
            {
                delete pElem;
                return true;
            }

            return false;
        }

    };

} // namespace mf
} // namespace surveon


#endif // SMF_CONTAINER_UTIL_H

