#ifndef GLVR_RENDER_OPERATION_H
#define GLVR_RENDER_OPERATION_H


#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRVertexData.h"
#include "GLVRIndexData.h"

namespace surveon
{
namespace glvr
{

class RenderOperation
{
public:
    enum OperationType
    {
        OT_POINTS = GL_POINTS,
        OT_LINES = GL_LINES,
        OT_LINE_LOOP = GL_LINE_LOOP,
        OT_TRIANGLES = GL_TRIANGLES,
        OT_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        OT_TRIANGLE_FAN =  GL_TRIANGLE_FAN
    };

public:
    RenderOperation(void);
    ~RenderOperation();

    void setOperationType(OperationType type)
    {
        m_OperationType = type;
    }

    void bind(const VertexDataSharedPtr pVertexData, const IndexDataSharedPtr pIndexData);

    void apply(void);

    VertexDataSharedPtr getVertexData(void)
    {
        return m_pVertexData;
    }

    IndexDataSharedPtr getIndexData(void)
    {
        return m_pIndexData;
    }

private:
    uint32  m_OperationType {GL_TRIANGLE_STRIP};
    uint32 m_VertexArrayObjectId;

    VertexDataSharedPtr m_pVertexData;
    IndexDataSharedPtr m_pIndexData;
};

} // namespace glvr
} // namespace surveon


#endif // GLVR_RENDER_OPERATION_H
