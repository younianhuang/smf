/*

*/

#ifndef GLVR_GPU_BUFFER_MANAGER_H
#define GLVR_GPU_BUFFER_MANAGER_H

#include "GLVRVertexBuffer.h"
#include "GLVRIndexBuffer.h"


namespace surveon
{
namespace glvr
{

class GPUBufferManager: public Singleton<GPUBufferManager>
{
    friend class Singleton<GPUBufferManager>;

public:

    //VertexBufferSharedPtr createVertexBuffer();

    IndexBufferSharedPtr createRectangleIndexBuffer();

private:
    GPUBufferManager();
    ~GPUBufferManager();

private:
    IndexBufferSharedPtr m_pRectangleIndexBuffer;
};

} // namespace glvr
} // namespace surveon




#endif // GLVR_GPU_BUFFER_MANAGER_H
