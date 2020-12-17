
#ifndef SMF_RTSP_SOURCE_FACTORY_H
#define SMF_RTSP_SOURCE_FACTORY_H

#include "SMFRTSPSource.h"

namespace surveon
{
namespace mf
{

/**
 */
class RTSPSourceFactory : public IURLSourceFactory
{
public:
    RTSPSourceFactory();
    virtual ~RTSPSourceFactory();

    virtual IMediaSource* create(const String& url);

    virtual void destroy(IMediaSource* pSource);

    virtual const String& getScheme(void) const;

private:


private:
    String m_Scheme;

    DerviedObjectFactory<IMediaSource, RTSPSource> m_FactoryImpl;

};

} // namespace mf
} // namespace surveon

#endif // SMF_RTSP_SOURCE_FACTORY_H



