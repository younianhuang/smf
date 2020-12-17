


#ifndef SMF_RTSP_SOURCE_H
#define SMF_RTSP_SOURCE_H

#include "SMFLIVE555Prerequisites.h"

typedef std::vector<surveon::mf::IStreamSource*> StreamSources;


namespace surveon
{
namespace mf
{



/**
 */
class RTSPSource : public  IMediaSource
{
public:
    RTSPSource();

    virtual ~RTSPSource();

    virtual void initialize(const String& url);

    virtual void shutdown(void);

    virtual void start(uint64 position = 0);

    virtual void stop(void);

    virtual void pause(void);

    virtual void resume(void);

    virtual StreamStatus getPacket(IMediaBuffer** ppMediaBuffer);

    virtual String getSource(void) const;

    virtual size_t getNumOfStream(void) const;

    virtual IStreamSource* getStreamSource(size_t index);

private:
    String m_URL;

};

} // namespace mf
} // namespace surveon

#endif // SMF_RTSP_SOURCE_H


extern void continueAfterClientCreation1(void);
extern Medium* createClient(UsageEnvironment& env, char const* URL, int verbosityLevel, char const* applicationName);
extern void _shutdown(int);

extern Boolean outputQuickTimeFile;
extern Boolean generateMP4Format;
extern Boolean outputAVIFile;
extern double duration;
extern unsigned fileOutputInterval;
extern char* username;
extern char* password;
extern unsigned fileSinkBufferSize;
extern UsageEnvironment* env;
extern int verbosityLevel;
extern char const* progName;
extern Medium* ourClient;
extern char const* clientProtocolName;
extern char const* streamURL;
extern double durationSlop;
extern Authenticator* ourAuthenticator;
extern MediaSession* session;
extern MediaSubsession* subsession;

