#ifndef GLVR_VERTEX_BUFFER_H
#define GLVR_VERTEX_BUFFER_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"

namespace surveon
{
namespace glvr
{

class VertexBuffer
{
public:
    VertexBuffer(size_t size, const void* pData = nullptr);
    ~VertexBuffer();

    void writeData(size_t offset, size_t size, const void* pData);

    void bind(void);

private:
    GLuint m_BufferObjectId;
    size_t m_Size;
};

typedef std::shared_ptr<VertexBuffer> VertexBufferSharedPtr;

} // namespace glvr
} // namespace surveon



#endif // GLVR_VERTEX_BUFFER_H
