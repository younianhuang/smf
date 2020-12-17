#ifndef GLVR_VERTEX_DATA_H
#define GLVR_VERTEX_DATA_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRVertexBuffer.h"
#include "GLVRVertexAttribute.h"

namespace surveon
{

namespace glvr
{

class VertexData
{
public:
    enum
    {
        VERTEX_LOCATION_POSITION = 0,
        VERTEX_LOCATION_TEX_COORD,
        VERTEX_LOCATION_VERTEX_COLOR,
        VERTEX_LOCATION_COUNT
    };

public:
    VertexData(size_t start, size_t num);

    void setVertexBuffer(uint32 location,  const VertexBufferSharedPtr pVertexBuffer,
                         const VertexAttributeSharedPtr pVertexAttribute);

    VertexBufferSharedPtr getVertexBuffer(uint32 location);

    size_t getVertexStart(void) const
    {
        return m_VertexStart;
    }

    size_t getNumOfVertex(void) const
    {
        return m_NumOfVertices;
    }

    void bind(void);

private:
    std::map<uint32, VertexBufferSharedPtr> m_VertexBuffers;
    std::map<uint32, VertexAttributeSharedPtr> m_VertexAttributes;

    /// The base vertex index to start from
    size_t m_VertexStart;
    /// The number of vertices used in this operation
    size_t m_NumOfVertices;
};

typedef std::shared_ptr<VertexData> VertexDataSharedPtr;


} // namespace glvr
} // namespace surveon

#endif // GLVR_VERTEX_DATA_H

