
#ifndef GLVR_VERTEX_ATTRIBUTE_H
#define GLVR_VERTEX_ATTRIBUTE_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"

namespace surveon
{
namespace glvr
{

class VertexAttribute
{
public:
    enum AttributeDataType
    {
        ADT_FLOAT2,
        ADT_FLOAT3,
        ADT_FLOAT4
    };

public:
    VertexAttribute(uint32 location, AttributeDataType  type);
    ~VertexAttribute();

    void bind(void);

    size_t getDataSize(void) const
    {
        return m_DataSize;
    }

private:

    ///location of the generic vertex attribute
    uint32 m_Location;
    /// the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally
    int32 m_NumOfComponent;
    ///the data type of each component in the array
    uint32 m_DataType;
    ///the size in bytes of each component in the array
    size_t m_DataSize;
};



typedef std::shared_ptr<VertexAttribute> VertexAttributeSharedPtr;

} // namespace glvr
} // namespace surveon



#endif // GLVR_VERTEX_ATTRIBUTE_H

