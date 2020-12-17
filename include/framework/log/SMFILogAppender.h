#ifndef SMF_ILOG_APPENDER_H
#define SMF_ILOG_APPENDER_H

#include "SMFLogPrerequisites.h"

namespace surveon
{
namespace mf
{
namespace log
{

class IAppender
{
public:

    virtual ~IAppender(void)   {}

    //virtual const String& getName(void) const = 0;

    virtual AppenderContext* getContext(void) = 0;
};


class IFileAppender : public IAppender
{
public:

    virtual ~IFileAppender(void)   {}

    //virtual const String& getFilename(void) const = 0;
};


class IRollingFileAppender : public IFileAppender
{
public:

    virtual ~IRollingFileAppender(void)   {}
};




} // namespace log
} // namespace mf
} // namespace surveon


#endif // SMF_ILOG_APPENDER_H

