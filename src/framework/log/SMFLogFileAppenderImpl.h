#ifndef SMF_LOG_FILE_APPENDER_IMPL_H
#define SMF_LOG_FILE_APPENDER_IMPL_H

#include "SMFLogImplPrerequisites.h"
#include "SMFLogAppenderContext.h"
#include "framework/log/SMFILogAppender.h"


namespace surveon
{
namespace mf
{
namespace log
{

class FileAppenderImpl : public IFileAppender
{
public:
    FileAppenderImpl(const String& filename);
    virtual ~FileAppenderImpl(void);

    virtual AppenderContext* getContext(void);
    //virtual const String& getFilename(void) const;

private:
    AppenderContext m_AppenderContext;

};



} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_LOG_FILE_APPENDER_IMPL_H


