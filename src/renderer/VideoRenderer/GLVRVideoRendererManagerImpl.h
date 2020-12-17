/*

*/

#ifndef SURVEON_GLVR_VIDEO_RENDERER_MANAGER_IMPL_H
#define SURVEON_GLVR_VIDEO_RENDERER_MANAGER_IMPL_H

#include <framework/SMFThread.h>
#include "GLVRITexture.h"
#include "GLVRIVideoRenderer.h"
#include "GLVRImplPrerequisites.h"
#include "GLVRVideoRendererImpl.h"
#include "GLVRTextureImpl.h"



namespace surveon
{
namespace glvr
{

class VideoRendererManagerImpl
{
public:
    VideoRendererManagerImpl(void);
    ~VideoRendererManagerImpl(void);

    void initialize(const String& logFilename);
    void shutdown(void);

    IVideoRenderer* createVideoRenderer(void);
    void destroyVideoRenderer(IVideoRenderer* pRender);
    void destroyAllVideoRenderers(void);

    ITexture* createTexture(void);
    void destroyTexture(ITexture* pTexture);
    void destroyAllTextures(void);

    Capabilities getCapabilities(void) const;   

/*
    Font* createFont();
    void destroyFont(Font*);

    Shader* createShader();
    void destroyShader(Shader*);
*/
private:
    void run(void);

    void initializeCommand(void);
    void releaseResourceCommand(void);

    IVideoRenderer* createVideoRendererCommand(void);
    void destroyVideoRendererCommand(IVideoRenderer* pRender);
    void destroyAllVideoRendererCommand(void);

    ITexture* createTextureCommand(void);

    void renderOneFrame(void);

    void waitForCommand(void);


private:
    std::set<VideoRendererImpl*> m_VideoRenderSet;

    typedef mf::DerviedObjectFactory<ITexture, TextureImpl> TextureFactory;
    TextureFactory m_TextureFactory;

    mutable mf::AsyncCommandQueue m_CommandQueue;    
    mutable AsyncCommandQueue::SyncControl m_CommandSyncControl;

    mf::Mutex m_Mutex;    
    mf::Thread m_Thread;

    bool m_Initialize;
    bool m_Run;
};

} // namespace glvr
} // namespace surveon




#endif // SURVEON_GLVR_VIDEO_RENDERER_MANAGER_IMPL_H

