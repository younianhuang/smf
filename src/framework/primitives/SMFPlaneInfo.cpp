

#include "framework/primitives/SMFPlaneInfo.h"


namespace surveon
{
namespace mf
{

PlaneInfo::PlaneInfo(void)
{
    m_PlaneDatas.reserve(MAX_PALNE_NUM);
}

PlaneInfo::~PlaneInfo(void)
{}

size_t PlaneInfo::getNumberOfPlanes(void) const
{
    return m_PlaneDatas.size();
}

const PlaneInfo::PlaneData& PlaneInfo::getPlaneData(size_t index) const
{
    assert(index < m_PlaneDatas.size());
    return m_PlaneDatas[index];
}

void PlaneInfo::addPlane(const PlaneData& plane)
{
    m_PlaneDatas.push_back(plane);
}

void PlaneInfo::setPixelBufferPtr(uint8* pixels)
{
    for(auto& planeData :  m_PlaneDatas)
    {
        planeData.pixels =  pixels;
        pixels += planeData.memorySize;
    }
}

void PlaneInfo::clearPlaneDatas(void)
{
    m_PlaneDatas.clear();
}


} // namespace mf
} // namespace surveon

