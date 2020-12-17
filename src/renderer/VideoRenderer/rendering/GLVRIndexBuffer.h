#ifndef GLVR_INDEX_BUFFER_H
#define GLVR_INDEX_BUFFER_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"


namespace surveon
{
namespace glvr
{

class IndexBuffer
{
public:
    IndexBuffer(size_t size, const void* pData = nullptr);
    ~IndexBuffer();

    void writeData(size_t offset, size_t size, const void* pData);

    void bind(void);

private:
    uint32 m_BufferObjectId;
    size_t m_Size;
};

typedef std::shared_ptr<IndexBuffer> IndexBufferSharedPtr;

} // namespace glvr
} // namespace surveon



#endif // GLVR_INDEX_BUFFER_H
