#ifndef SMF_NONCOPY_ABLE
#define SMF_NONCOPY_ABLE

#include "framework/SMFPrerequisites.h"

namespace surveon
{
namespace mf
{

class SMF_API NonCopyable
{
protected:
    /** Default constructor
    */
    NonCopyable() {}


private:
    /** Disabled copy constructor
    */
    NonCopyable(const NonCopyable&);

    /** Disabled assignment operator
    */
    NonCopyable& operator =(const NonCopyable&);
};


} // namespace mf
} // namespace surveon




#endif // SMF_NONCOPY_ABLE

