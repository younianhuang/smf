
#ifndef SMF_IMEDIA_TRANSFORM_H
#define SMF_IMEDIA_TRANSFORM_H

#include "framework/SMFPrerequisites.h"
#include "framework/primitives/SMFIMediaBuffer.h"
#include "framework/primitives/SMFMediaType.h"
#include "framework/primitives/SMFAttributes.h"
#include "framework/pipeline/SMFIDeviceManager.h"

namespace surveon
{
namespace mf
{

//=====================================================================
// global attributes for the same type MediaTransform
//=====================================================================

/// enalbe/disable hardware acceleration
/// type: int
const AttributeId  ATTRI_NUMBER_OF_THREAD = "ATTRI_NUMBER_OF_THREAD";

//=====================================================================
// attributes for each MediaTransform instance
//=====================================================================

/// enalbe/disable hardware acceleration
/// type: bool
const AttributeId  ATTRI_HARDWARD_ACCEL = "ATTRI_HARDWARD_ACCEL";

/// type: int
const AttributeId  ATTRI_VO_TARGET = "ATTRI_VO_TARGET";

/// type: IDeviceManager
const AttributeId  ATTRI_DEVICE_MANAGER = "ATTRI_DEVICE_MANAGER";

/**
 */
class SMF_API IMediaTransform
{
public:
    enum Status
    {
        ST_OK,
        ST_FAILED,
        ST_NOT_ACCEPT_DATA,
        ST_ACCEPT_DATA,
        ST_NEED_MORE_INPUT,
        ST_OUTPUT_DATA_READY
    };

    struct StreamInfo
    {
        uint32 width {0};
        uint32 height {0};
        MediaType mediaType;
    };

    struct Statistics
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
    virtual ~IMediaTransform(void) {}

    virtual const String& getTypeId(void) const = 0;

    virtual void initialize(void) = 0;

    virtual void shutdown(void) = 0;

    virtual void setInputType(const MediaType& mediaType) = 0;

    virtual Status getInputStatus(void) const = 0;

    virtual void processInput(IMediaBuffer* pBuffer) = 0;

    virtual void getOutputStreamInfo(StreamInfo& streamInfo) const = 0;

    virtual void setOutputType(const MediaType& mediaType) = 0;

    virtual Status getOutputStatus(void) const = 0;

    virtual void processOutput(IMediaBuffer* pBuffer) = 0;

    virtual void getStatistics(Statistics& stat) const = 0;

    virtual Attributes* getAttributes(void) = 0;

};

} // namespace mf
} // namespace surveon

#endif // SMF_IMEDIA_TRANSFORM_H



