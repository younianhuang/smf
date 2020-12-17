#ifndef SMF_COREPREREQUISITES_H
#define SMF_COREPREREQUISITES_H

#include "SMFCoreLog.h"
#include "framework/SMFException.h"

#define SMF_CORE_THROW_EXCEPT(ExceptType, desc)  { \
        surveon::mf::StringStream ss;   \
        ss << desc;  \
        ExceptType ex(ss.str(), __FILE__, __LINE__, __FUNCTION__);   \
        SMF_CORE_LOG_ERROR(ex.getLogDescription());  \
        throw ex; }

#endif // SMF_COREPREREQUISITES_H

