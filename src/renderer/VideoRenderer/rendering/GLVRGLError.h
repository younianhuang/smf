#ifndef GLVR_GL_ERROR_H
#define GLVR_GL_ERROR_H

#include "GLVRRenderEnginePrerequisites.h"



namespace surveon
{
namespace glvr
{

class GLError
{
public:
    static const String& getString(GLenum errorCode);

    static String toString(GLenum errorCode);
};


#define GLVR_GL_CHECK(glfun)   \
        glfun;  \
        GLVR_LOG_TRACE(#glfun); \
        { \
            GLenum errCode = glGetError(); \
            if (errCode != GL_NO_ERROR)  { \
                GLVR_THROW_EXCEPT(GLVROpenGLErrorException, \
                    "Fail to call " << #glfun << ": [" << GLError::getString (errCode) <<"]"); \
            }   \
        }

#define GLVR_GL_TRACE(glfun)   \
        glfun;  \
        GLVR_LOG_TRACE(#glfun); \
        {   \
            GLenum errCode = glGetError(); \
            if (errCode != GL_NO_ERROR)             { \
                GLVR_LOG_ERROR("Fail to call " << #glfun << ": [" << GLError::getString (errCode) <<"]"); \
            }   \
        }

} // namespace glvr
} // namespace surveon


#endif // GLERROR_H
