#ifndef SMF_ISTREAM_DESCRIPTOR_H
#define SMF_ISTREAM_DESCRIPTOR_H


#include "framework/primitives/SMFMediaType.h"


namespace surveon
{
namespace mf
{

class IStreamDescriptor
{
public:
    virtual ~IStreamDescriptor(void)    {}

    virtual MediaMajorType getMajorType(void) const = 0;

    virtual MediaSubType getSubType(void) const = 0;
};


} // namespace mf
} // namespace surveon



#endif // SMF_ISTREAM_DESCRIPTOR_H


