#ifndef SMF_IDEVICE_MANAGER_H
#define SMF_IDEVICE_MANAGER_H

#include "framework/SMFPrerequisites.h"


namespace surveon
{
namespace mf
{

class IDeviceManager
{
public:
    virtual ~IDeviceManager(void)    {}

    virtual void* getNativeDeviceHandle(void) = 0;

    virtual uint getAdaptorNumber(void) const = 0;
};


} // namespace mf
} // namespace surveon

#endif // IDeviceManager


