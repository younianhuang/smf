/*

*/

#ifndef GLVR_VIDEO_RENDERER_MANAGER_H
#define GLVR_VIDEO_RENDERER_MANAGER_H

#include "GLVRIVideoRenderer.h"
#include "GLVRITexture.h"
#include "GLVRCapabilities.h"

namespace surveon
{
namespace glvr
{

class VideoRendererManagerImpl;

/** The root class of the video render system.
 * The class represents a starting point for the client application.
 * From here, the application can access to other classes in the system.
 * The instance of VideoRendererManager must be initialized before any other operations are called.
 */
class GLVR_API VideoRendererManager
{
public:
    static VideoRendererManager& getInstance(void);
    static VideoRendererManager* getInstancePtr(void);
    static void destroyInstance(void);

    /** Initializes the render system .
     * @remarks
     *          This function must be called in GUI event loop thread.
     *
     * @param logFilename the fullpath of log file.
     * If empty, no log will be written.
    */
    void initialize(const String& logFilename = "glvr.log");

    /** Shuts down the render system.
     * @remarks
     *          This function must be called in GUI event loop thread.
    */
    void shutdown(void);

    IVideoRenderer* createVideoRenderer(void);
    void destroyVideoRenderer(IVideoRenderer* pVideoRenderer);
    void destroyAllVideoRenderers(void);

    ITexture* createTexture(void);
    void destroyTexture(ITexture* pTexture);
    void destroyAllTextures(void);

    Capabilities getCapabilities(void) const;

private:
        VideoRendererManager(void);
        VideoRendererManager(VideoRendererManager& rhs);
         ~VideoRendererManager(void);

private:
        VideoRendererManagerImpl* m_pImpl;

        static VideoRendererManager* ms_pInstance;
};

} // namespace glvr
} // namespace surveon




#endif // GLVR_VIDEO_RENDERER_MANAGER_H

