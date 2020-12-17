#ifndef SMF_SINGLETON_H
#define SMF_SINGLETON_H

#include "framework/SMFPrerequisites.h"
#include "framework/thread/SMFMutex.h"
#include "SMFNonCopyable.h"


namespace surveon
{
namespace mf
{

/** Generic singleton class.
*/
template <class T, class MutexType = NullMutex>
class Singleton : public NonCopyable
{

public:
    static T& getInstance(void);

    static T* getInstancePtr(void);

    static void destroyInstance(void);

protected:
    Singleton(void) {}

private:
    static T* ms_Instance;
    static MutexType ms_Mutex;
};


} // namespace mf
} // namespace surveon

#include "SMFSingletonDetail.h"

#endif // SMF_SINGLETON_H

