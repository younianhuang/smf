
#ifndef SMF_ISTREAM_SOURCE_H
#define SMF_ISTREAM_SOURCE_H

#include "framework/SMFPrerequisites.h"
#include "framework/primitives/SMFIMediaBuffer.h"
#include "framework/primitives/SMFMediaType.h"


namespace surveon
{
namespace mf
{

/**
 */
class IStreamSource
{
public:
    struct SMF_API Statistics
    {
        float lastFPS {0};
        float avgFPS {0};
        size_t bitrate {0};

        void reset()
        {
            lastFPS = 0.0f;
            avgFPS = 0.0f;
            bitrate = 0;
        }
    };

public:
    virtual ~IStreamSource(void) {}

    virtual int getStreamIndex(void) const = 0;

    virtual void setActive(bool active) = 0;

    virtual bool isActive(void) const = 0;

    virtual const MediaType& getMediaType(void) const = 0;

    virtual void getStatistics(Statistics& stat) const = 0;

};

} // namespace mf
} // namespace surveon

#endif // SMF_ISTREAM_SOURCE_H


