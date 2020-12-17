#include "GLVRRetangleShape.h"
#include "GLVRGPUBufferManager.h"

namespace surveon
{
namespace glvr
{

RetangleShape::RetangleShape(RenderTarget* pRenderTarget, const mf::FloatRect& rect) :
    Shape(pRenderTarget),
    m_Rect(rect)
{
    setRect(rect);
    m_UpdateFillColor = true;
    m_UpdateTextureGeometry = true;
}

RetangleShape::~RetangleShape()
{

}

void RetangleShape::setRect(const mf::FloatRect& rect)
{
    m_Rect = rect;

    /* NOTE:
     * Points for outline only, different with rect vertices
    */
    /*
        0-----1
        |    /|
        |  /  |
        |/    |
        3-----2
    */
    if(m_Points.size() == 0)
        setPointCount(4);

    m_Points[0] = Vector2f(rect.left, rect.top);
    m_Points[1] = Vector2f(rect.right, rect.top);
    m_Points[2] = Vector2f(rect.right, rect.bottom);
    m_Points[3] = Vector2f(rect.left, rect.bottom);

    notifyGeometoryChanged();
}


void RetangleShape::createHardwareBuffers(void)
{
    VertexDataSharedPtr pVertexData(new VertexData(0, 4));

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
    IndexBufferSharedPtr pIndexBuffer = GPUBufferManager::getInstancePtr()->createRectangleIndexBuffer();

    IndexDataSharedPtr pIndexData(new IndexData(0, 6, pIndexBuffer));

    m_RenderOperation.bind(pVertexData, pIndexData);

    m_RenderOperation.setOperationType(RenderOperation::OT_TRIANGLE_STRIP);
}


void RetangleShape::updateGeometry(void)
{
    /* CCW
        0-----2
        |    /|
        |  /  |
        |/    |
        1-----3
    */
    Shape::updateGeometry();

    mf::FloatRect NDCRect = m_pRenderTarget->getViewport().screenCoordToNDC(m_Rect);

     Vector3f vertexArray[] =
     {
         Vector3f(NDCRect.left, NDCRect.top, 0.0f),
         Vector3f(NDCRect.left, NDCRect.bottom, 0.0f),
         Vector3f(NDCRect.right, NDCRect.top, 0.0f),
         Vector3f(NDCRect.right, NDCRect.bottom, 0.0f),
     };

     m_RenderOperation.getVertexData()->getVertexBuffer(VertexData::VERTEX_LOCATION_POSITION)->
             writeData(0, sizeof(vertexArray), vertexArray);
}

void RetangleShape::updateTextureGeometry(void)
{
    /* CCW
        0-----2
        |    /|
        |  /  |
        |/    |
        1-----3
    */
     Vector2f vertexArray[] =
     {
         Vector2f(m_TextureCoordRect.left, m_TextureCoordRect.top),
         Vector2f(m_TextureCoordRect.left, m_TextureCoordRect.bottom),
         Vector2f(m_TextureCoordRect.right, m_TextureCoordRect.top),
         Vector2f(m_TextureCoordRect.right, m_TextureCoordRect.bottom),
     };

     m_RenderOperation.getVertexData()->getVertexBuffer(VertexData::VERTEX_LOCATION_TEX_COORD)->
             writeData(0, sizeof(vertexArray), vertexArray);
}


} // namespace glvr
} // namespace surveon



