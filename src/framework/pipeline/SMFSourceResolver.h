
#ifndef SMF_SOURCE_RESOLVER_H
#define SMF_SOURCE_RESOLVER_H

#include "framework/pipeline/SMFIMediaSourceFactory.h"
#include "SMFDynamicLibrary.h"
#include "SMFCorePrerequisites.h"

namespace surveon
{
namespace mf
{


/**
 */
class SourceResolver
{

public:
    SourceResolver();
    ~SourceResolver();

    void initialize(void);
    void shutdown(void);

    IMediaSource* createSourceFromFile(const String& filename);
    void destroyFileSource(IMediaSource* pSource);

    void registerFileSourceFactory(IFileSourceFactory* pFactory);
    void unregisterFileSourceFactory(IFileSourceFactory* pFactory);

    IMediaSource* createSourceFromURL(const String& url);
    void destroyURLSource(IMediaSource* pSource);

    void registerURLSourceFactory(IURLSourceFactory* pFactory);
    void unregisterURLSourceFactory(IURLSourceFactory* pFactory);

    void destroyMediaSource(IMediaSource* pSource);

private:

    IFileSourceFactory* findFileSourceFactory(const String& filename);

    IFileSourceFactory* getFileSourceFactory(const String& filename);

    IURLSourceFactory* findURLSourceFactory(const String& url);

    IURLSourceFactory* getURLSourceFactory(const String& url);

private:
    bool m_Initialized;

    typedef std::map<String, IFileSourceFactory*> FileSourceFactoryMap;
    FileSourceFactoryMap m_FileSourceFactoryMap;

    typedef std::map<String, IURLSourceFactory*> URLSourceFactoryMap;
    URLSourceFactoryMap m_URLSourceFactoryMap;
};

} // namespace mf
} // namespace surveon



#endif // SMF_SOURCE_RESOLVER_H







