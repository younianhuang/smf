#ifndef SMF_LOGGER_LEVEL_H
#define SMF_LOGGER_LEVEL_H

#include "SMFLogPrerequisites.h"

namespace surveon
{
namespace mf
{
namespace log
{

/**
* \typedef int LogLevel
* Defines the minimum set of priorities recognized by the system,
* that is {@link #FATAL LOG_LEVEL_}, {@link #ERROR LOG_LEVEL_}, {@link
* #WARN LOG_LEVEL_}, {@link #INFO LOG_LEVEL_}, {@link #DEBUG LOG_LEVEL_},
* and {@link #TRACE LOG_LEVEL_}.
*/
typedef int LogLevel;

/** \var extern const LogLevel OFF LOG_LEVEL_
* The <code>OFF LOG_LEVEL_</code> LogLevel is used during configuration to
* turn off logging. */
SMF_API extern const LogLevel LOG_LEVEL_OFF;

/** \var extern const LogLevel FATAL LOG_LEVEL_
* The <code>FATAL LOG_LEVEL_</code> LogLevel designates very severe error
* events that will presumably lead the application to abort.  */
SMF_API extern const LogLevel LOG_LEVEL_FATAL;

/** \var extern const LogLevel ERROR LOG_LEVEL_
* The <code>ERROR LOG_LEVEL_</code> LogLevel designates error events that
* might still allow the application to continue running. */
SMF_API extern const LogLevel LOG_LEVEL_ERROR;

/** \var extern const LogLevel WARN LOG_LEVEL_
* The <code>WARN LOG_LEVEL_</code> LogLevel designates potentially harmful
* situations. */
SMF_API extern const LogLevel LOG_LEVEL_WARN;

/** \var extern const LogLevel INFO LOG_LEVEL_
* The <code>INFO LOG_LEVEL_</code> LogLevel designates informational
* messages  that highlight the progress of the application at
* coarse-grained  level. */
SMF_API extern const LogLevel LOG_LEVEL_INFO;

/** \var extern const LogLevel DEBUG LOG_LEVEL_
* The <code>DEBUG LOG_LEVEL_</code> LogLevel designates fine-grained
* informational events that are most useful to debug an application. */
SMF_API extern const LogLevel LOG_LEVEL_DEBUG;

/** \var extern const LogLevel TRACE LOG_LEVEL_
* The <code>TRACE LOG_LEVEL_</code> LogLevel is used to "trace" entry
* and exiting of methods. */
SMF_API extern const LogLevel LOG_LEVEL_TRACE;

/** \var extern const LogLevel ALL LOG_LEVEL_
* The <code>ALL LOG_LEVEL_</code> LogLevel is used during configuration to
* turn on all logging. */
SMF_API extern const LogLevel  LOG_LEVEL_ALL;

/** \var extern const LogLevel NOT_SET LOG_LEVEL_
* The <code>NOT_SET LOG_LEVEL_</code> LogLevel is used to indicated that
* no particular LogLevel is desired and that the default should be used.
*/
SMF_API extern const LogLevel  LOG_LEVEL_NOT_SET;



} // namespace log
} // namespace mf
} // namespace surveon

#endif // SMF_LOGGER_LEVEL_H

