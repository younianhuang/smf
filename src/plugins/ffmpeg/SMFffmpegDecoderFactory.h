#ifndef SMF_FFMPEG_DECODER_FACTORY_H
#define SMF_FFMPEG_DECODER_FACTORY_H

#include "framework/pipeline/SMFIMediaTransformFactory.h"
#include "SMFffmpegException.h"
#include "SMFffmpegDecoder.h"

namespace surveon
{
namespace mf
{


class ffmpegDecoderFactory: public IMediaTransformFactory
{
public:
    ffmpegDecoderFactory();

    virtual ~ffmpegDecoderFactory(void);

    virtual IMediaTransform* create(void);

    virtual void destroy(IMediaTransform* pTransform);

    virtual const String& getTypeId(void) const;

    virtual Attributes* getAttributes(void);

private:
    void onAttributeChanged(const String& name);

    void attributeChangedCommand(const String& name);

private:
    mutable Mutex m_Mutex;

    DerviedObjectFactory<IMediaTransform, ffmpegVideoDecoder> m_FactoryImpl;

    WorkQueue m_WorkQueue;

    AsyncCommandQueue* m_pCommandQueue;

    Attributes m_Attributes;
};

} // namespace mf
} // namespace surveon



#endif // SMF_FFMPEG_DECODER_FACTORY_H
