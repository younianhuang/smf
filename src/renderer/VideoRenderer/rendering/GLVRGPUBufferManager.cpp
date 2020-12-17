#include "GLVRGPUBufferManager.h"

namespace surveon
{
namespace glvr
{

GPUBufferManager::GPUBufferManager()
{

}

GPUBufferManager::~GPUBufferManager()
{
    GLVR_LOG_INFO("destroy GPUBufferManager.");
}

IndexBufferSharedPtr GPUBufferManager::createRectangleIndexBuffer()
{
    if(!m_pRectangleIndexBuffer)
    {
        uint16 indexArray[] = {0, 1, 2, 1, 2, 3};
        //uint16 indexArray[] = {0, 3, 1, 3, 1, 2};
        m_pRectangleIndexBuffer.reset(new IndexBuffer(sizeof(indexArray), indexArray));
    }
    return m_pRectangleIndexBuffer;
}

} // namespace glvr
} // namespace surveon



