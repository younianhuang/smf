#include "GLVRVertexData.h"


namespace surveon
{
namespace glvr
{

VertexData::VertexData(size_t start, size_t num) :
    m_VertexStart(start),
    m_NumOfVertices(num)
{

}

void VertexData::setVertexBuffer(uint32 location,  const VertexBufferSharedPtr pVertexBuffer,
                                 const VertexAttributeSharedPtr pVertexAttribute)
{
    m_VertexBuffers[location] = pVertexBuffer;
    m_VertexAttributes[location] = pVertexAttribute;
}

VertexBufferSharedPtr VertexData::getVertexBuffer(uint32 location)
{
    return m_VertexBuffers[location];
}

void VertexData::bind(void)
{
    for(auto& pair : m_VertexBuffers)
    {
        pair.second->bind();
        m_VertexAttributes[pair.first]->bind();
    }
}

} // namespace glvr
} // namespace surveon

