#include "GLVRRenderQueue.h"

namespace surveon
{
namespace glvr
{


RenderQueue::RenderQueue()
{

}

RenderQueue::~RenderQueue()
{

}

void RenderQueue::addRenderable(uint8 zorder, Renderable* pRenderable)
{
    m_RenderGroupMap[zorder].add(pRenderable);
}

RenderQueue::Iterator RenderQueue::getRenderGroupMapIterator(void)
{
    return Iterator(m_RenderGroupMap);
}

} // namespace glvr
} // namespace surveon







