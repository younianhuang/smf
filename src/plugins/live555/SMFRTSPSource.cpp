#include "SMFRTSPSource.h"


char watchVariable = 0;
size_t numOfStream = 0;

StreamSources streamSources;


namespace surveon
{
namespace mf
{





//===============================================================================

RTSPSource::RTSPSource()
{

}

RTSPSource::~RTSPSource()
{
    if(!streamSources.empty())
    {
        for(StreamSources::iterator it=streamSources.begin(); it!=streamSources.end(); it++)
        {
            delete *it;
            *it = nullptr;
        }
    }
    streamSources.clear();
}

void RTSPSource::initialize(const String& url)
{
    m_URL = url;

    // Command line settings
    streamURL = m_URL.c_str();
//    outputQuickTimeFile = True;
//    generateMP4Format = True;
    outputAVIFile = True;
    duration = 2.0f;
    durationSlop = 0.0f;
    fileOutputInterval = 10;
    username = "admin";
    password = "admin";
    fileSinkBufferSize = 200000;

    ourAuthenticator = new Authenticator(username, password);

    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    env = BasicUsageEnvironment::createNew(*scheduler);

    // Create (or arrange to create) our client object:
    ourClient = createClient(*env, streamURL, verbosityLevel, progName);
    if (ourClient == NULL) {
      *env << "Failed to create " << clientProtocolName << " client: " << env->getResultMsg() << "\n";
      _shutdown(1);
    }
    continueAfterClientCreation1();

//    MediaSubsessionIterator iter(*session);
//    MediaSubsession *subsession;
//    while((subsession = iter.next()) != NULL)
//    {
//        if(subsession->readSource() == NULL) continue; // was not initiated

//        if(strcmp(subsession->mediumName(), "video") == 0)
//        {
//            RTSPVideoStreamSource* pRTSPVideoStreamSource = new  RTSPVideoStreamSource(subsession);
//            m_StreamSources.push_back(pRTSPVideoStreamSource);
//        }
//        else
//        {
//            m_StreamSources.push_back(nullptr);
//        }
//    }

    // All subsequent activity takes place within the event loop:
    env->taskScheduler().doEventLoop( &watchVariable ); // If "watchVariable" stopEventLoop not NULL,
                                                        // then we return from this routine
    env->reclaim();
    delete scheduler;
}

void RTSPSource::shutdown(void)
{


}

void RTSPSource::start(uint64 position)
{

}

void RTSPSource::stop(void)
{
}

void RTSPSource::pause(void)
{

}

void RTSPSource::resume(void)
{

}

RTSPSource::StreamStatus RTSPSource::getPacket(IMediaBuffer** ppMediaBuffer)
{
    return SS_OK;
}


size_t RTSPSource::getNumOfStream(void) const
{
    return numOfStream;
}

IStreamSource* RTSPSource::getStreamSource(size_t index)
{
    return streamSources[index];
}

String RTSPSource::getSource(void) const
{
    return m_URL;
}





} // namespace mf
} // namespace surveon



