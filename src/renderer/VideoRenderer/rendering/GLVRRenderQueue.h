#ifndef GLVR_RENDER_QUEUE_H
#define GLVR_RENDER_QUEUE_H

#include "GLVRRenderable.h"


namespace surveon
{
namespace glvr
{

class RenderQueue
{
public:
    typedef mf::Queue<Renderable*> RenderGroup;
    typedef std::map<uint8, RenderGroup> RenderGroupMap;
    typedef MapIterator<RenderGroupMap> Iterator;

public:
    RenderQueue();

    ~RenderQueue();

    void addRenderable(uint8 zorder, Renderable* pRenderable);

    Iterator getRenderGroupMapIterator(void);

private:
    RenderGroupMap m_RenderGroupMap;
};


} // namespace glvr
} // namespace surveon

#endif // GLVR_RENDER_QUEUE_H


#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H




#endif // RENDERQUEUE_H
