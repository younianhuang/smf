#ifndef SMF_SYSTEM_MEMORY_BUFFER_FACTORY_H
#define SMF_SYSTEM_MEMORY_BUFFER_FACTORY_H

#include "framework/primitives/SMFIMediaBufferFactory.h"
#include "framework/thread/SMFThread.h"
#include "framework/SMFException.h"
#include "framework/util/SMFFactory.h"
#include "framework/util/SMFContainerUtil.h"
#include "SMFSystemMemoryVideoBuffer.h"


namespace surveon
{
namespace mf
{



class SystemMemoryMediaBufferFactory : public IMediaBufferFactory
{
public:

    IMediaBuffer* createMediaBuffer(size_t size, uint8 aligment = 1);
    void destroyMediaBuffer(IMediaBuffer* pBuffer);

    IVideoBuffer* createVideoBuffer(uint16 width, uint16 height, MediaSubType format, uint8 aligment = 1);
    void destroyVideoBuffer(IVideoBuffer* pBuffer);

private:
    HierarchyObjectFactory<IMediaBuffer, Mutex> m_MediaBufferFactory;
    HierarchyObjectFactory<IVideoBuffer, Mutex> m_VideoBufferFactory;
};

} // namespace mf
} // namespace surveon

#endif // SMF_SYSTEM_MEMORY_BUFFER_FACTORY_H

