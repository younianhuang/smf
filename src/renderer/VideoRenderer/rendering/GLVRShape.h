#ifndef GLVR_SHAPE_H
#define GLVR_SHAPE_H

#include "GLVRRenderable.h"
#include "GLVRRenderTarget.h"
#include "GLVRVector2.h"
#include "GLVRColor.h"


namespace surveon
{
namespace glvr
{

class Outline: public Renderable
{
public:
    Outline(RenderTarget* pRenderTarget) ;

    virtual ~Outline()  {}
};


class Shape : public Renderable
{
public:
    Shape(RenderTarget* pRenderTarget);

    virtual ~Shape();

    virtual void show(void);

    virtual void hide(void);

    virtual void update(void);

    virtual void setPointCount (uint32 pointCount);

    virtual uint32 getPointCount (void) const;

    virtual void setPoint (uint32 index, const Vector2f& point);

    virtual const Vector2f& getPoint(uint32 index) const;

    virtual void setTextureCoordRect(const mf::FloatRect& rect);

    virtual const mf::FloatRect& getTextureCoordRect(void) const;

    virtual void setFillColor(const Color& color);

    virtual void setOutlineColor(const Color& color);

    virtual void setOutlineThickness(int thickness);

    virtual void setZOrder(uint8 zorder);

    virtual void notifyViewportChanged(void);

    virtual const mf::FloatRect& getBoundaryRect(void) const;

protected:

    virtual void createHardwareBuffers(void) {}

    virtual void updateGeometry(void);

    virtual void updateTextureGeometry(void) {}

    void updateFillColor(void);

    void createOutlineHardwareBuffers();

    void updateOutilneGeometry(void);

    void updateOutilneColor(void);

    void caculateBoundaryRect(void);

    void caculateCentroid(void);


    float convertZOrderToNDC(void)
    {
        return 1.0f - static_cast<float>(m_ZOrder) /127.5f;
    }

    void notifyGeometoryChanged(void)
    {
        m_UpdateGeometry = true;

        m_UpdateOutilneGeometry = true;
    }

protected:
    /// Shape point positions in screen coordinate
    std::vector<Vector2f> m_Points;

    /// Outline point positions in screen coordinate
    std::vector<Vector2f> m_OutlinePoints;

    /// boundary rect in screen coordinate
    FloatRect m_BoundaryRect;

    /// Centroid of this Shape in screen coordinate
    Vector2f m_Centroid;

    FloatRect m_TextureCoordRect;
    Color m_FillColor;
    Color m_OutlineColor;

    int m_OutlineThickness;

    bool m_UpdateGeometry;
    bool m_UpdateTextureGeometry;
    bool m_UpdateFillColor;
    bool m_UpdateOutilneGeometry;
    bool m_UpdateOutilneColor;

    Outline m_Outline;
};

} // namespace glvr
} // namespace surveon


#endif // GLVR_SHAPE_H


