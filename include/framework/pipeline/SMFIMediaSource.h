
#ifndef SMF_IMEDIA_SOURCE_H
#define SMF_IMEDIA_SOURCE_H

#include "framework/SMFPrerequisites.h"
#include "framework/primitives/SMFIMediaBuffer.h"
#include "framework/pipeline/SMFIStreamSource.h"

namespace surveon
{
namespace mf
{

/**
 */
class IMediaSource
{
public:
    enum StreamStatus
    {
        SS_OK,
        SS_FAILED,
        SS_PAUSE,
        SS_STOP,
        SS_END_OF_STREAM
    };

public:
    virtual ~IMediaSource(void) {}

    virtual void initialize(const String& source) = 0;

    virtual void shutdown(void) = 0;

    virtual void start(uint64 position = 0) = 0;

    virtual void stop(void) = 0;

    virtual void pause(void) = 0;

    virtual void resume(void) = 0;

    virtual StreamStatus getPacket(IMediaBuffer** ppMediaBuffer) = 0;

    virtual String getSource(void) const = 0;

    virtual size_t getNumOfStream(void) const = 0;

    virtual IStreamSource* getStreamSource(size_t index) = 0;

};

} // namespace mf
} // namespace surveon

#endif // SMF_IMEDIA_SOURCE_H



