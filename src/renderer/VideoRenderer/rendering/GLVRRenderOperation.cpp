#include "GLVRRenderOperation.h"

namespace surveon
{
namespace glvr
{

RenderOperation::RenderOperation(void)
{    
    GLVR_GL_CHECK(glGenVertexArrays(1, &m_VertexArrayObjectId));
    GLVR_LOG_DEBUG("Generate VertexArray Object: " << m_VertexArrayObjectId);
}

RenderOperation::~RenderOperation()
{
    GLVR_GL_TRACE(glDeleteVertexArrays(1, &m_VertexArrayObjectId));
    GLVR_LOG_DEBUG("Delete VertexArray Object: " << m_VertexArrayObjectId);
}

void RenderOperation::bind(const VertexDataSharedPtr pVertexData, const IndexDataSharedPtr pIndexData)
{
    assert(pVertexData);

    GLVR_LOG_TRACE("Bind VertexArrays Object: " << m_VertexArrayObjectId);

    m_pVertexData = pVertexData;
    m_pIndexData = pIndexData;

    GLVR_GL_CHECK(glBindVertexArray(m_VertexArrayObjectId));
    m_pVertexData->bind();

    if(m_pIndexData)
        m_pIndexData->bind();

    GLVR_GL_CHECK(glBindVertexArray(0));
}

void RenderOperation::apply(void)
{
    GLVR_LOG_TRACE("Apply VertexArrays Object: " << m_VertexArrayObjectId);

    GLVR_GL_CHECK(glBindVertexArray(m_VertexArrayObjectId));

    if(m_pIndexData)
    {
        GLVR_GL_CHECK(glDrawElements(m_OperationType,
                                     static_cast<GLsizei>(m_pIndexData->getNumOfIndex()),
                                     GL_UNSIGNED_SHORT,
                                     (const void*)m_pIndexData->getIndexStart()));
     }
    else
    {

    }

    GLVR_GL_CHECK(glBindVertexArray(0));
}


} // namespace glvr
} // namespace surveon

