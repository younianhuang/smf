#ifndef SMF_REFERENCE_COUNT_H
#define SMF_REFERENCE_COUNT_H

#include "framework/SMFPrerequisites.h"
#include "framework/thread/SMFMutex.h"



namespace surveon
{
namespace mf
{

class SMF_API ReferenceCount
{
public:
    ReferenceCount(void);

    ~ReferenceCount(void);

    bool isReferenced(void) const;

    void addReference(void);

    void releaseReference(void);

    void waitUntilFree(void);

private:
    size_t m_Count;
    RecursiveMutex m_Mutex;
    ConditionVariable m_ConditionVariable;
};


} // namespace mf
} // namespace surveon


#endif // SMF_REFERENCE_COUNT_H



