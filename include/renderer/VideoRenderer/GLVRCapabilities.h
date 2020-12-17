#ifndef GLVR_CAPABILITY_H
#define GLVR_CAPABILITY_H

#include "GLVRPrerequisites.h"


namespace surveon
{
namespace glvr
{

struct GLVR_API Capabilities
{
    String glVersion;
    String shaderVersion;
    String gpuModel;
};

} // namespace glvr
} // namespace surveon

#endif // GLVRCAPABILITY_H
