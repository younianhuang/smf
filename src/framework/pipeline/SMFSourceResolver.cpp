#include "SMFSourceResolver.h"

namespace surveon
{
namespace mf
{

SourceResolver::SourceResolver() :
    m_Initialized(false)
{
    shutdown();
}

SourceResolver::~SourceResolver()
{

}

void SourceResolver::initialize(void)
{
    if(m_Initialized)
        return;

    m_Initialized = true;
}

void SourceResolver::shutdown(void)
{
    if(m_Initialized)
    {
        m_FileSourceFactoryMap.clear();

        m_Initialized = false;
    }
}

void SourceResolver::registerFileSourceFactory(IFileSourceFactory* pFactory)
{
    assert(pFactory);
    m_FileSourceFactoryMap[pFactory->getFileType()] = pFactory;
}

void SourceResolver::unregisterFileSourceFactory(IFileSourceFactory* pFactory)
{
    assert(pFactory);
    m_FileSourceFactoryMap.erase(m_FileSourceFactoryMap.find(pFactory->getFileType()));
}

IMediaSource* SourceResolver::createSourceFromFile(const String& filename)
{
    IFileSourceFactory* pFactory=  getFileSourceFactory(filename);
    IMediaSource* pSource= pFactory->create(filename);
    return pSource;
}

void SourceResolver::destroyFileSource(IMediaSource* pSource)
{
    if(pSource == nullptr)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException, "null pointer is invalid");
    }

    IFileSourceFactory* pFactory=  getFileSourceFactory(pSource->getSource());

    pFactory->destroy(pSource);
}

IMediaSource* SourceResolver::createSourceFromURL(const String& url)
{
    IURLSourceFactory* pFactory=  getURLSourceFactory(url);
    IMediaSource* pSource= pFactory->create(url);
    return pSource;
}

void SourceResolver::destroyURLSource(IMediaSource* pSource)
{
    if(pSource == nullptr)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException, "null pointer is invalid");
    }

    IURLSourceFactory* pFactory=  getURLSourceFactory(pSource->getSource());

    pFactory->destroy(pSource);

}

void SourceResolver::registerURLSourceFactory(IURLSourceFactory* pFactory)
{
    assert(pFactory);
    m_URLSourceFactoryMap[pFactory->getScheme()] = pFactory;
}

void SourceResolver::unregisterURLSourceFactory(IURLSourceFactory* pFactory)
{
    assert(pFactory);
    m_URLSourceFactoryMap.erase(m_URLSourceFactoryMap.find(pFactory->getScheme()));
}

void SourceResolver::destroyMediaSource(IMediaSource* pSource)
{
    if(pSource == nullptr)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException, "null pointer is invalid");
    }

    IFileSourceFactory* pFileSourceFactory = findFileSourceFactory(pSource->getSource());
    if(pFileSourceFactory)
    {
        pFileSourceFactory->destroy(pSource);
        return;
    }

    IURLSourceFactory* pURLSourceFactory = findURLSourceFactory(pSource->getSource());
    if(pURLSourceFactory)
    {
        pURLSourceFactory->destroy(pSource);
        return;
    }

    SMF_CORE_THROW_EXCEPT(InvalidParametersException, "invalid source pointer");
}

//===========================================================================

IFileSourceFactory* SourceResolver::findFileSourceFactory(const String& filename)
{
    size_t found = filename.find_last_of('.');
    if(found == String::npos)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException,  filename << " is valid filename");
    }

    String filetype = filename.substr(found+1);

    auto it = m_FileSourceFactoryMap.find(filetype);
    if(it == m_FileSourceFactoryMap.end())
    {
        return nullptr;
    }

    return it->second;
}

IFileSourceFactory* SourceResolver::getFileSourceFactory(const String& filename)
{
    size_t found = filename.find_last_of('.');
    if(found == String::npos)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException,  filename << " is valid filename");
    }

    String filetype = filename.substr(found+1);

    auto it = m_FileSourceFactoryMap.find(filetype);
    if(it == m_FileSourceFactoryMap.end())
    {
        SMF_CORE_THROW_EXCEPT(UnsupportFormatException,  filetype << " is not support");
    }

    return it->second;
}

IURLSourceFactory* SourceResolver::findURLSourceFactory(const String& url)
{
    size_t found = url.find_last_of(':');
    if(found == String::npos)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException, url << " is valid url");
    }

    String scheme = url.substr(0, found);

    auto it = m_URLSourceFactoryMap.find(scheme);
    if(it == m_URLSourceFactoryMap.end())
    {
        return nullptr;
    }

    return it->second;
}

IURLSourceFactory* SourceResolver::getURLSourceFactory(const String& url)
{
    size_t found = url.find_last_of(':');
    if(found == String::npos)
    {
        SMF_CORE_THROW_EXCEPT(InvalidParametersException, url << " is valid url");
    }

    String scheme = url.substr(0, found);

    auto it = m_URLSourceFactoryMap.find(scheme);
    if(it == m_URLSourceFactoryMap.end())
    {
        SMF_CORE_THROW_EXCEPT(UnsupportFormatException, scheme << " is not support");
    }

    return it->second;
}


} // namespace mf
} // namespace surveon



