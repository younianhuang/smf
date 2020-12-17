#ifndef SMF_IMEDIA_TRANSFORM_FACTORY_H
#define SMF_IMEDIA_TRANSFORM_FACTORY_H

#include "framework/pipeline/SMFIMediaTransform.h"
#include "framework/primitives/SMFAttributes.h"

namespace surveon
{
namespace mf
{

class IMediaTransformFactory
{
public:
    virtual ~IMediaTransformFactory(void)    {}

    virtual IMediaTransform* create(void) = 0;

    virtual void destroy(IMediaTransform* pTransform)  = 0;

    virtual const String& getTypeId(void) const = 0;

    virtual Attributes* getAttributes(void) = 0;
};




} // namespace mf
} // namespace surveon


#endif // SMF_IMEDIA_TRANSFORM_FACTORY_H

