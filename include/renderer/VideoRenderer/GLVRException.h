#ifndef GLVR_EXCEPTION_H
#define GLVR_EXCEPTION_H

#include "GLVRPrerequisites.h"
#include "GLVRLog.h"
#include <framework/SMFException.h>

namespace surveon
{
namespace glvr
{

class  GLVR_API GLVRException : public Exception
{
public:
    GLVRException(const String& typeName, const String& description, const String&file, int line, const String& function) :
        Exception(typeName, description, file, line, function)
    {}
};

class  GLVR_API GLVRInvalidParametersException : public GLVRException
{
public:
    GLVRInvalidParametersException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVRInvalidParametersException", description, file, line, function)
    {}
};

class  GLVR_API GLVRInvalidOperationException : public GLVRException
{
public:
    GLVRInvalidOperationException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVRInvalidOperationException", description, file, line, function)
    {}
};

class  GLVR_API GLVRInvalidStateException : public GLVRException
{
public:
    GLVRInvalidStateException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVRInvalidStateException", description, file, line, function)
    {}
};

class  GLVR_API GLVRInternalErrorException : public GLVRException
{
public:
    GLVRInternalErrorException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVRInternalErrorException", description, file, line, function)
    {}
};

class  GLVR_API GLVROpenGLErrorException : public GLVRException
{
public:
    GLVROpenGLErrorException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVROpenGLErrorException", description, file, line, function)
    {}
};

class  GLVR_API GLVROpenGLSLCompileErrorException : public GLVRException
{
public:
    GLVROpenGLSLCompileErrorException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVROpenGLSLCompileErrorException", description, file, line, function)
    {}
};

class  GLVR_API GLVROpenGLSLLinkErrorException : public GLVRException
{
public:
    GLVROpenGLSLLinkErrorException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVROpenGLSLLinkErrorException", description, file, line, function)
    {}
};

class  GLVR_API GLVRNotImplementException : public GLVRException
{
public:
    GLVRNotImplementException(const String& description, const String&file, int line, const String& function) :
        GLVRException("GLVRNotImplementException", description, file, line, function)
    {}
};

#define GLVR_THROW_EXCEPT(ExceptType, desc)  { \
        surveon::mf::StringStream ss;   \
        ss << desc;  \
        ExceptType ex(ss.str(), __FILE__, __LINE__, __FUNCTION__);   \
        GLVR_LOG_ERROR(ex.getLogDescription())  \
        throw ex; }


} // namespace glvr
} // namespace surveon


#endif // GLVR_EXCEPTION_H

