#ifndef GLVR_SHADER_H
#define GLVR_SHADER_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLError.h"

namespace surveon
{
namespace glvr
{

class Shader
{
public:
    enum ShaderType
    {
        ST_FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        ST_VERTEX_SHADER = GL_VERTEX_SHADER
    };

    enum Status
    {
        ST_UNLOADED,
        ST_LOADED,
        ST_COMPILED
    };

public:
    Shader(ShaderType type);
    ~Shader(void);

    void loadFromString(const String& source);
    //void loadFromFile(const String& filename);

    void compile(void);

    uint32 getId(void) const
    {
        return m_ShaderObjectId;
    }

    Status getStatus(void) const
    {
        return m_Status;
    }

private:
    GLenum m_ShaderType;
    GLuint m_ShaderObjectId;
    Status m_Status;
};

typedef std::shared_ptr<Shader> ShaderSharedPtr;


} // namespace glvr
} // namespace surveon




#endif // GLVR_SHADER_H
