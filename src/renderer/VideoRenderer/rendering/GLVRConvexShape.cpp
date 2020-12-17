#include "GLVRConvexShape.h"
#include "GLVRGPUBufferManager.h"

namespace surveon
{
namespace glvr
{

ConvexShape::ConvexShape(RenderTarget* pRenderTarget, uint32 pointCount) :
    Shape(pRenderTarget)    
{
    setPointCount(pointCount);   
}

ConvexShape::~ConvexShape()
{

}

void ConvexShape::createHardwareBuffers(void)
{
    VertexDataSharedPtr pVertexData(new VertexData(0, m_Points.size() + 1));

    // position buffer
    VertexAttributeSharedPtr pVertexAttribute(new VertexAttribute(
                                                  VertexData::VERTEX_LOCATION_POSITION, VertexAttribute::ADT_FLOAT3));

    VertexBufferSharedPtr pVertexBuffer(new VertexBuffer(
                                            pVertexData->getNumOfVertex() * pVertexAttribute->getDataSize()));

    pVertexData->setVertexBuffer(VertexData::VERTEX_LOCATION_POSITION,
                                 pVertexBuffer, pVertexAttribute);

    // texture coordinate buffer
    VertexAttributeSharedPtr pTextureCoordVertexAttribute(new VertexAttribute(
                                                  VertexData::VERTEX_LOCATION_TEX_COORD, VertexAttribute::ADT_FLOAT2));

    VertexBufferSharedPtr pTexureCoordVertexBuffer(new VertexBuffer(
                                            pVertexData->getNumOfVertex() * pTextureCoordVertexAttribute->getDataSize()));

    pVertexData->setVertexBuffer(VertexData::VERTEX_LOCATION_TEX_COORD,
                                 pTexureCoordVertexBuffer, pTextureCoordVertexAttribute);

    // vertex color
    VertexAttributeSharedPtr pVertexColorAttribute(new VertexAttribute(
                                                  VertexData::VERTEX_LOCATION_VERTEX_COLOR, VertexAttribute::ADT_FLOAT4));

    VertexBufferSharedPtr pVertexColorBuffer(new VertexBuffer(
                                            pVertexData->getNumOfVertex() * pVertexColorAttribute->getDataSize()));

    pVertexData->setVertexBuffer(VertexData::VERTEX_LOCATION_VERTEX_COLOR,
                                 pVertexColorBuffer, pVertexColorAttribute);

    // index buffer 
    std::vector<uint16> indexArray(m_Points.size() *  3);

    for(size_t pointIndex=0, indexOfIndexArray = 0; pointIndex<m_Points.size(); pointIndex++, indexOfIndexArray+=3)
    {
        indexArray[indexOfIndexArray] = 0;

        indexArray[indexOfIndexArray+1] = pointIndex >= m_Points.size() -1 ? 1 :  static_cast<uint16>(pointIndex + 2);

        indexArray[indexOfIndexArray+2] = static_cast<uint16>(pointIndex+1);
    }

    IndexBufferSharedPtr pIndexBuffer(new IndexBuffer(indexArray.size() * sizeof(uint16), indexArray.data()));

    IndexDataSharedPtr pIndexData(new IndexData(0, indexArray.size(), pIndexBuffer));

    m_RenderOperation.bind(pVertexData, pIndexData);

    m_RenderOperation.setOperationType(RenderOperation::OT_TRIANGLE_FAN);
}


void ConvexShape::updateGeometry(void)
{
    /*
        1------2
        |\    / |
        |   0   |
        | /   \ |
        4------3

        1------4
        |\    / |
        |   0   |
        | /   \ |
        2------3
    */
    Shape::updateGeometry();

    // convert points to NDC Vertices
    std::vector<Vector3f> vertexArray(m_Points.size() + 1);

    //use centroid as first vertex
    const Viewport& viewport = m_pRenderTarget->getViewport();
    vertexArray[0] = viewport.screenCoordToNDC(m_Centroid.x, m_Centroid.y, 0.0f);

    for(size_t i=0; i< m_Points.size(); i++)
    {
        Vector2f& screenPos =  m_Points[i];
        vertexArray[i+1] = viewport.screenCoordToNDC(screenPos.x, screenPos.y, 0.0f);
    }

    m_RenderOperation.getVertexData()->getVertexBuffer(VertexData::VERTEX_LOCATION_POSITION)->
            writeData(0, vertexArray.size() * sizeof(Vector3f), vertexArray.data());
}



} // namespace glvr
} // namespace surveon



