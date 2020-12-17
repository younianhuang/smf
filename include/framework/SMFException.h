
#ifndef SMF_EXCEPTION_H
#define SMF_EXCEPTION_H

#include "SMFPrerequisites.h"

namespace surveon
{
namespace mf
{

class SMF_API Exception : public std::exception
{
public:


public:
    Exception(const String& typeName,
                      const String& description,
                      const String&file,
                      int line,
                      const String& function);

    virtual ~Exception(void);

    const String& getDescription(void) const;

    const String& getLogDescription(void) const;

    /** Returns a string with the full description of this error.
    */
    virtual const String& getFullDescription(void) const;

    virtual const char* what(void) const SMF_NO_EXCEPT;

protected:
        int m_Line;
        String m_TypeName;
        String m_Description;
        String m_File;
        String m_Function;
        mutable String m_LogDescription;
        mutable String m_FullDescription;
};


class  SMF_API InvalidParametersException : public Exception
{
public:
    InvalidParametersException(const String& description, const String&file, int line, const String& function) :
        Exception("InvalidParametersException", description, file, line, function)
    {}
};

class  SMF_API InvalidOperationException : public Exception
{
public:
    InvalidOperationException(const String& description, const String&file, int line, const String& function) :
        Exception("InvalidOperationException", description, file, line, function)
    {}
};

class  SMF_API UnimplementedException : public Exception
{
public:
    UnimplementedException(const String& description, const String&file, int line, const String& function) :
        Exception("UnimplementedException", description, file, line, function)
    {}
};

class  SMF_API InvalidStateException : public Exception
{
public:
    InvalidStateException(const String& description, const String&file, int line, const String& function) :
        Exception("InvalidStateException", description, file, line, function)
    {}
};

class  SMF_API InvalidTypeException : public Exception
{
public:
    InvalidTypeException(const String& description, const String&file, int line, const String& function) :
        Exception("InvalidTypeException", description, file, line, function)
    {}
};

class  SMF_API InternalErrorException : public Exception
{
public:
    InternalErrorException(const String& description, const String&file, int line, const String& function) :
        Exception("InternalErrorException", description, file, line, function)
    {}
};

class  SMF_API OpenFileErrorException : public Exception
{
public:
    OpenFileErrorException(const String& description, const String&file, int line, const String& function) :
        Exception("OpenFileErrorException", description, file, line, function)
    {}
};

class  SMF_API UnsupportFormatException : public Exception
{
public:
    UnsupportFormatException(const String& description, const String&file, int line, const String& function) :
        Exception("UnsupportFormatException", description, file, line, function)
    {}
};


#define SMF_THROW_EXCEPT(ExceptType, desc)   \
        surveon::mf::StringStream ss;   \
        ss << desc;  \
        ExceptType ex(ss.str(), __FILE__, __LINE__, __FUNCTION__);  \
        throw ex;


#define SMF_THROW_AND_LOG_EXCEPT(logger, ExceptType, desc)   \
        ExceptType ex(desc, __FILE__, __LINE__, __FUNCTION__);   \
        SMF_LOG_ERROR(logger, ex.getLogDescription())  \
        throw ex;

} // namespace mf
} // namespace surveon

#endif // SMF_EXCEPTION_H

