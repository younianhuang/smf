#ifndef SMF_IMEDIA_BUFFER_FACTORY_H
#define SMF_IMEDIA_BUFFER_FACTORY_H


#include "framework/SMFPrerequisites.h"
#include "framework/primitives/SMFIMediaBuffer.h"
#include "framework/primitives/SMFIVideoBuffer.h"


namespace surveon
{
namespace mf
{

class IMediaBufferFactory
{
public:
    virtual ~IMediaBufferFactory(void)   {}

    virtual IMediaBuffer* createMediaBuffer(size_t size, uint8 aligment = 1) = 0;
    virtual void destroyMediaBuffer(IMediaBuffer* pBuffer) = 0;

    virtual IVideoBuffer* createVideoBuffer(uint16 width, uint16 height, MediaSubType format, uint8 aligment = 1) = 0;
    virtual void destroyVideoBuffer(IVideoBuffer* pBuffer) = 0;

};


SMF_API IMediaBufferFactory* getSystemMemoryMediaBufferFactory(void);

} // namespace mf
} // namespace surveon

#endif // SMF_IMEDIA_BUFFER_FACTORY_H

