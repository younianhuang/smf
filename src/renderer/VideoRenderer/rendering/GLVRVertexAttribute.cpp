#include "GLVRVertexAttribute.h"

namespace surveon
{
namespace glvr
{

VertexAttribute::VertexAttribute(uint32 location, AttributeDataType  type) :
    m_Location(location)
{
    switch(type)
    {
    case ADT_FLOAT2:
        m_NumOfComponent = 2;
        m_DataType = GL_FLOAT;
        m_DataSize = sizeof(float) * m_NumOfComponent;
        break;
    case ADT_FLOAT3:
        m_NumOfComponent = 3;
        m_DataType = GL_FLOAT;
        m_DataSize = sizeof(float) * m_NumOfComponent;
        break;
    case ADT_FLOAT4:
        m_NumOfComponent = 4;
        m_DataType = GL_FLOAT;
        m_DataSize = sizeof(float) * m_NumOfComponent;
        break;
    }
}

VertexAttribute::~VertexAttribute()
{

}

 void VertexAttribute::bind(void)
 {
     GLVR_GL_CHECK(glEnableVertexAttribArray(m_Location));
     GLVR_GL_CHECK(glVertexAttribPointer(m_Location, m_NumOfComponent, m_DataType, GL_FALSE, 0, 0));
 }


} // namespace glvr
} // namespace surveon



