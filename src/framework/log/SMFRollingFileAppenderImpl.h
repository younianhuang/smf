#ifndef SMF_ROLLINGFILE_APPENDER_IMPL_H
#define SMF_ROLLINGFILE_APPENDER_IMPL_H

#include "SMFLogImplPrerequisites.h"
#include "SMFLogAppenderContext.h"
#include "framework/log/SMFILogAppender.h"


namespace surveon
{
namespace mf
{
namespace log
{

class RollingFileAppenderImpl : public IRollingFileAppender
{
public:
    RollingFileAppenderImpl(const String& filename);
    virtual ~RollingFileAppenderImpl(void);

    virtual AppenderContext* getContext(void);
    //virtual const String& getFilename(void) const;

private:
    AppenderContext m_AppenderContext;

};



} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_ROLLINGFILE_APPENDER_IMPL_H





