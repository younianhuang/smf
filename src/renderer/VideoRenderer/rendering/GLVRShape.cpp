#include "GLVRShape.h"
#include "GLVRGPUBufferManager.h"
#include "GLVRGPUProgramManager.h"

namespace surveon
{
namespace glvr
{

Outline::Outline(RenderTarget* pRenderTarget) :
    Renderable(pRenderTarget)
{
    m_pMaterial.reset(new Material);

    m_pMaterial->setProgram(GPUProgramManager::getInstance().getDefaultGPUProgram());
}


//===================================================================================

Shape::Shape(RenderTarget* pRenderTarget) :
    Renderable(pRenderTarget),
    m_TextureCoordRect(0.0f, 0.0f, 1.0f, 1.0f),
    m_FillColor(Color::White),
    m_OutlineColor(Color::White),
    m_OutlineThickness(0),
    m_UpdateGeometry(false),
    m_UpdateTextureGeometry(false),
    m_UpdateFillColor(false),
    m_UpdateOutilneGeometry(false),
    m_UpdateOutilneColor(false),
    m_Outline(pRenderTarget)
{
    m_Outline.hide();
}

Shape::~Shape()
{

}

void Shape::show(void)
{
    Renderable::show();
    if(m_OutlineThickness)
    {
        m_Outline.show();
    }
}

void Shape::hide(void)
{
    Renderable::hide();
    m_Outline.hide();
}

void Shape::update(void)
{
    if(m_UpdateGeometry)
    {
        updateGeometry();
        m_UpdateGeometry = false;
    }

    if(m_UpdateTextureGeometry)
    {
        updateTextureGeometry();
        m_UpdateTextureGeometry = false;
    }

    if(m_UpdateFillColor)
    {
        updateFillColor();
        m_UpdateFillColor = false;
    }

    if(m_UpdateOutilneGeometry && m_Outline.getRenderOperation().getVertexData())
    {
        updateOutilneGeometry();
        m_UpdateOutilneGeometry = false;
    }

    if(m_UpdateOutilneColor && m_Outline.getRenderOperation().getVertexData())
    {
        updateOutilneColor();
        m_UpdateOutilneColor = false;
    }
}

void Shape::setZOrder(uint8 zorder)
{
    Renderable::setZOrder(zorder);
    m_Outline.setZOrder(zorder);
}

void Shape::notifyViewportChanged(void)
{
    notifyGeometoryChanged();
}

void Shape::setFillColor(const Color& color)
{
    m_FillColor = color;
    m_UpdateFillColor = true;
}

void Shape::setOutlineColor(const Color& color)
{
    m_OutlineColor = color;
    m_UpdateOutilneColor = true;
}

void Shape::setOutlineThickness(int thickness)
{
    if(m_OutlineThickness == thickness)
        return;

    m_OutlineThickness = thickness;

    if(m_OutlineThickness)
    {
        if(!m_Outline.getRenderOperation().getVertexData())
        {            
            m_Outline.setZOrder(getZOrder());

            createOutlineHardwareBuffers();

            m_UpdateOutilneGeometry = true;
        }

        m_Outline.setVisible(isShow());
    }
    else
    {
        m_Outline.hide();
    }        
}

void Shape::setTextureCoordRect(const mf::FloatRect& rect)
{
    m_TextureCoordRect = rect;
    m_UpdateTextureGeometry = true;
}

const mf::FloatRect& Shape::getTextureCoordRect(void) const
{
    return m_TextureCoordRect;
}

void Shape::setPointCount (uint32 pointCount)
{
    if(pointCount < 3)
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Point Count " << pointCount
                          << " is not allowed.");
    }

    if(m_Points.size() != pointCount)
    {
        m_Points.resize(pointCount);
        createHardwareBuffers();

        m_UpdateFillColor = true;

        m_OutlinePoints.resize(pointCount * 2);

        if(m_OutlineThickness)
        {
            createOutlineHardwareBuffers();
            m_UpdateOutilneColor = true;
        }
    }
}

uint32 Shape::getPointCount (void) const
{
    return static_cast<uint32>(m_Points.size());
}

void Shape::setPoint (uint32 index, const Vector2f &point)
{
    if(index >= m_Points.size())
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "index " << index
                          << " is over the point count of this Convex.");
    }

    m_Points[index] = point;

    notifyGeometoryChanged();
}

const Vector2f& Shape::getPoint(uint32 index)  const
{
    if(index >= m_Points.size())
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "index " << index
                          << " is over the point count of this Convex.");
    }

    return m_Points[index];
}

const mf::FloatRect& Shape::getBoundaryRect(void) const
{
    return m_BoundaryRect;
}

//==========================================================================================
void Shape::updateGeometry(void)
{
    caculateBoundaryRect();
    caculateCentroid();
}

void Shape::updateFillColor(void)
{
    VertexDataSharedPtr pVertexData = m_RenderOperation.getVertexData();

    std::vector<Color> vertexColorArray(pVertexData->getNumOfVertex(), m_FillColor);

    pVertexData->getVertexBuffer(VertexData::VERTEX_LOCATION_VERTEX_COLOR)->
            writeData(0, vertexColorArray.size() * sizeof(Color) , vertexColorArray.data());
}

void Shape::createOutlineHardwareBuffers(void)
{
    VertexDataSharedPtr pVertexData(new VertexData(0, m_OutlinePoints.size()));

    // position buffer
    VertexAttributeSharedPtr pVertexAttribute(new VertexAttribute(
                                                  VertexData::VERTEX_LOCATION_POSITION, VertexAttribute::ADT_FLOAT3));

    VertexBufferSharedPtr pVertexBuffer(new VertexBuffer(
                                            pVertexData->getNumOfVertex() * pVertexAttribute->getDataSize()));

    pVertexData->setVertexBuffer(VertexData::VERTEX_LOCATION_POSITION,
                                 pVertexBuffer, pVertexAttribute);

    // vertex color
    VertexAttributeSharedPtr pVertexColorAttribute(new VertexAttribute(
                                                  VertexData::VERTEX_LOCATION_VERTEX_COLOR, VertexAttribute::ADT_FLOAT4));

    VertexBufferSharedPtr pVertexColorBuffer(new VertexBuffer(
                                            pVertexData->getNumOfVertex() * pVertexColorAttribute->getDataSize()));

    pVertexData->setVertexBuffer(VertexData::VERTEX_LOCATION_VERTEX_COLOR,
                                 pVertexColorBuffer, pVertexColorAttribute);

    // index buffer
    std::vector<uint16> indexArray(m_OutlinePoints.size() *  3);

    for(size_t pointIndex=0, indexOfIndexArray = 0; pointIndex<m_OutlinePoints.size(); pointIndex++, indexOfIndexArray+=3)
    {
        indexArray[indexOfIndexArray] = static_cast<uint16>(pointIndex);

        indexArray[indexOfIndexArray+1] = static_cast<uint16>((pointIndex+1) % m_OutlinePoints.size());

        indexArray[indexOfIndexArray+2] = static_cast<uint16>((pointIndex + 2) % m_OutlinePoints.size());
    }

    IndexBufferSharedPtr pIndexBuffer(new IndexBuffer(indexArray.size() * sizeof(uint16), indexArray.data()));

    IndexDataSharedPtr pIndexData(new IndexData(0, indexArray.size(), pIndexBuffer));


    RenderOperation& renderOP = m_Outline.getRenderOperation();
    renderOP.bind(pVertexData, pIndexData);

    renderOP.setOperationType(RenderOperation::OT_TRIANGLE_STRIP);
}

void Shape::updateOutilneGeometry(void)
{
    for(size_t i=0; i<m_Points.size(); i++ )
    {
        Vector2f& point = m_Points[i];

        Vector2f dir = point - m_Centroid;
        dir.normalize();

        m_OutlinePoints[i*2] = point;
        m_OutlinePoints[i*2+1] = point + dir * m_OutlineThickness;
    }

    std::vector<Vector3f> vertexArray(m_OutlinePoints.size());

    //use centroid as first vertex
    const Viewport& viewport = m_pRenderTarget->getViewport();

    for(size_t i=0; i< m_OutlinePoints.size(); i++)
    {
        Vector2f& screenPos =  m_OutlinePoints[i];
        vertexArray[i] = viewport.screenCoordToNDC(screenPos.x, screenPos.y, 0.0);
    }

    m_Outline.getRenderOperation().getVertexData()->getVertexBuffer(VertexData::VERTEX_LOCATION_POSITION)->
            writeData(0, vertexArray.size() * sizeof(Vector3f), vertexArray.data());
}

void Shape::updateOutilneColor(void)
{
    std::vector<Color> vertexColorArray(m_OutlinePoints.size(), m_OutlineColor);

    m_Outline.getRenderOperation().getVertexData()->getVertexBuffer(VertexData::VERTEX_LOCATION_VERTEX_COLOR)->
            writeData(0, vertexColorArray.size() * sizeof(Color) , vertexColorArray.data());
}


void Shape::caculateBoundaryRect(void)
{
    Vector2f& pos0 = m_Points[0];

    m_BoundaryRect.left = pos0.x;
    m_BoundaryRect.top = pos0.y;
    m_BoundaryRect.right = pos0.x;
    m_BoundaryRect.bottom = pos0.y;

    for(size_t i=1; i<m_Points.size(); i++ )
    {
        Vector2f& pos = m_Points[i];

        if(m_BoundaryRect.left > pos.x)
            m_BoundaryRect.left = pos.x;

        if(m_BoundaryRect.top > pos.y)
            m_BoundaryRect.top = pos.y;

        if(m_BoundaryRect.right < pos.x)
            m_BoundaryRect.right = pos.x;

        if(m_BoundaryRect.bottom < pos.y)
            m_BoundaryRect.bottom = pos.y;
    }
}

void Shape::caculateCentroid(void)
{
    m_Centroid.x = m_BoundaryRect.left + m_BoundaryRect.getWidth()/2;
    m_Centroid.y = m_BoundaryRect.top + m_BoundaryRect.getHeight()/2;
}



} // namespace glvr
} // namespace surveon



