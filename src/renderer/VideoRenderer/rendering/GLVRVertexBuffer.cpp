#include "GLVRVertexBuffer.h"


namespace surveon
{
namespace glvr
{

VertexBuffer::VertexBuffer(size_t size, const void* pData) :
    m_Size(size)
{
    assert(size);

    GLVR_GL_CHECK(glGenBuffers(1, &m_BufferObjectId));

    GLVR_LOG_DEBUG("Generate BufferObject " << m_BufferObjectId);

    GLVR_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_BufferObjectId));
    GLVR_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, pData, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLVR_LOG_DEBUG("Delete BufferObject " << m_BufferObjectId);
    GLVR_GL_TRACE(glDeleteBuffers(1, &m_BufferObjectId));
}

void VertexBuffer::writeData(size_t offset, size_t size, const void* pData)
{
    if(offset + size > m_Size)
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "offset or size is over boundary of this buffer ");
    }

    GLVR_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_BufferObjectId));
    GLVR_GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, offset, size, pData));
}

void VertexBuffer::bind(void)
{
    GLVR_LOG_DEBUG("Bind VertexBufferObject " << m_BufferObjectId);
    GLVR_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_BufferObjectId));
}

} // namespace glvr
} // namespace surveon
