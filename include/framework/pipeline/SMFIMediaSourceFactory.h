#ifndef SMF_SMF_IMEDIA_SOURCE_FACTORY_H
#define SMF_SMF_IMEDIA_SOURCE_FACTORY_H


#include "SMFIMediaSource.h"


namespace surveon
{
namespace mf
{

class IFileSourceFactory
{
public:
    virtual ~IFileSourceFactory(void)    {}

    virtual IMediaSource* create(const String& filename) = 0;

    virtual void destroy(IMediaSource* pSource)  = 0;

    virtual const String& getFileType(void) const  = 0;
};


class IURLSourceFactory
{
public:
    virtual ~IURLSourceFactory(void)    {}

    virtual IMediaSource* create(const String& url) = 0;

    virtual void destroy(IMediaSource* pSource)  = 0;

    virtual const String& getScheme(void) const  = 0;
};

} // namespace mf
} // namespace surveon



#endif // SMF_SMF_IMEDIA_SOURCE_FACTORY_H




