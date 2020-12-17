#ifndef SMF_VIDEO_PLANE_INFO_H
#define SMF_VIDEO_PLANE_INFO_H


#include "framework/SMFPrerequisites.h"
#include "SMFPixelFormat.h"


namespace surveon
{
namespace mf
{

class SMF_API PlaneInfo
{
public:
    struct PlaneData
    {
        uint8* pixels;
        int width;
        int height;
        int pitch;
        size_t memorySize;

        PlaneData(void) :
            pixels(nullptr), width(0), height(0), pitch(0), memorySize(0)
        {}

        PlaneData(uint8* pixels_, int width_, int height_, int pitch_, size_t size) :
            pixels(pixels_), width(width_), height(height_), pitch(pitch_), memorySize(size)
        {}

    };

public:
    static const size_t MAX_PALNE_NUM = 3;

public:
    PlaneInfo(void);

    ~PlaneInfo(void);

    size_t getNumberOfPlanes(void) const;

    const PlaneData& getPlaneData(size_t index) const;

    void addPlane(const PlaneData& plane);

    void setPixelBufferPtr(uint8* pixels);

    void clearPlaneDatas(void);

private:
    std::vector<PlaneData> m_PlaneDatas;

};


} // namespace mf
} // namespace surveon



#endif // SMF_VIDEO_PLANE_INFO_H

