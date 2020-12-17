/*

*/

#ifndef GLVR_IVIDEO_RENDERER_H
#define GLVR_IVIDEO_RENDERER_H

#include "GLVRPrerequisites.h"
#include "GLVRIVideoPane.h"
#include "GLVRIRetangleShape.h"
#include "GLVRIConvexShape.h"
#include "GLVRISprite.h"

namespace surveon
{
namespace glvr
{

/** Class for manages the rendering to a render window.
 * Video render manages what to render and how to render to a specific window.
 * Video render can create Renderable objects like video, image retangle shape etc... for rendering something.
 * And It also manages how to mix these Renderable objects.
*/
class IVideoRenderer
{
public:
    struct Statistics
    {
        float lastFPS;
        float avgFPS;
    };

public:

    virtual ~IVideoRenderer(void) {}

    /** Initialize this video renderer. This function must be called before other operation.
     * @param
                handle The handle of the Widnow this VideoRenderer will render to.
     * @remarks
     *          This function must be called in GUI thread.
    */
    virtual void initialize(mf::WindowHandle handle) = 0;

    /** Shutdown this video renderer. This will release all the resources of this video renderer.
    */
    virtual void shutdown(void) = 0;
    virtual bool isInitialized(void) const = 0;

    virtual void repaint(void) = 0;

    /** Sets the render area of the target window.
      * @param rect area in window coordinate.
      * @remarks
      *     To keep the same ratio, this fuction must be called when window size changed.
    */
    virtual void setViewport(const mf::FloatRect& rect) = 0;
    inline  void setViewport(float left, float top, float right, float bottom)
    {
        setViewport(mf::FloatRect(left, top, right, bottom));
    }

    virtual IVideoPane* createVideoPane() = 0;
    virtual void destroyVideoPane(IVideoPane*) = 0;
    virtual void destroyAllVideoPanes(void) = 0;

    /** Flush all video data of all Video Panes.
    */
    virtual void flush(void) = 0;

    virtual IRetangleShape* createRetangleShape(const mf::FloatRect& rect) = 0;
    inline  IRetangleShape*  createRetangleShape(float left, float top, float right, float bottom)
    {
        return createRetangleShape(mf::FloatRect(left, top, right, bottom));
    }
    virtual void destroyRetangleShape(IRetangleShape*) = 0;
    virtual void destroyAllRetangleShapes(void) = 0;

    virtual IConvexShape* createConvexShape(uint32 pointCount) = 0;
    virtual void destroyConvexShape(IConvexShape*) = 0;
    virtual void destroyAllConvexShapes(void) = 0;

    virtual ISprite* createSprite(const mf::FloatRect& rect) = 0;
    inline  ISprite* createSprite(float left, float top, float right, float bottom)
    {
        return createSprite(mf::FloatRect(left, top, right, bottom));
    }
    virtual void destroySprite(ISprite*) = 0;
    virtual void destroyAllSprites(void) = 0;

    virtual Statistics getStatistics(void) const = 0;

    virtual void setPresentationClock(mf::IPresentationClock* pPresentationClock) = 0;

    /** Creates the GPU hardware buffer for the render target of hardware decode acceleration. */
    virtual mf::IVideoBuffer* createGPUHardwareVideoBuffer(uint16 width, uint16 height) = 0;
    virtual void destroyGPUHardwareVideoBuffer(mf::IVideoBuffer* pVideoBuffer) = 0;

    virtual IDeviceManager* getDeviceManager(void) = 0;
};

} // namespace glvr
} // namespace surveon




#endif // GLVR_IVIDEO_RENDERER_H

