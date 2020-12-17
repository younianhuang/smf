#ifndef GLVR_INDEX_DATA_H
#define GLVR_INDEX_DATA_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRIndexBuffer.h"


namespace surveon
{

namespace glvr
{

class IndexData
{

public:
    IndexData(size_t start, size_t num, IndexBufferSharedPtr pIndexBuffer) :
        m_pIndexBuffer(pIndexBuffer), m_IndexStart(start), m_NumOfVertices(num)
    {}

    void setIndexBuffer(IndexBufferSharedPtr pIndexBuffer)
    {
        m_pIndexBuffer = pIndexBuffer;
    }

    IndexBufferSharedPtr getIndexBuffer(void)
    {
        return m_pIndexBuffer;
    }

    size_t getIndexStart(void) const
    {
        return m_IndexStart;
    }

    size_t getNumOfIndex(void) const
    {
        return m_NumOfVertices;
    }

    void bind(void)
    {
        if(m_pIndexBuffer)
            m_pIndexBuffer->bind();
    }

private:
    IndexBufferSharedPtr m_pIndexBuffer;

    /// The base Index index to start from
    size_t m_IndexStart;
    /// The number of vertices used in this operation
    size_t m_NumOfVertices;
};


typedef std::shared_ptr<IndexData> IndexDataSharedPtr;

} // namespace glvr
} // namespace surveon

#endif // GLVR_INDEX_DATA_H

