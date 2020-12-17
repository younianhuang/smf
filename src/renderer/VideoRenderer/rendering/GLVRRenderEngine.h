
#ifndef GLVR_RENDER_ENGINE_H
#define GLVR_RENDER_ENGINE_H

#include "GLVRRenderWindow.h"
#include "GLVRGPUBufferManager.h"
#include "GLVRTextureManager.h"
#include "GLVRGPUProgramManager.h"
#include "GLVRCapabilities.h"
#include "GLVRRenderWindowFactory.h"


namespace surveon
{
namespace glvr
{

/*
struct GLInfo
{
    String glVersion;
    String shaderVersion;
    String gpuModel;
};
*/


class RenderEngine : public mf::Singleton<RenderEngine>
{
    friend class Singleton<RenderEngine>;

public:
    class Listener
    {
    public:
        Listener()  {}
        virtual ~Listener(){}

        virtual void onRenderSystemInitialized(void) {}
    };

public:

    void initialize(void);
    void shutdown(void);

    void addListener(Listener* pListener);
    void removeListener(Listener* pListener);

    RenderWindow* createRenderWindow(mf::WindowHandle handle);
    void destroyRenderWindow(RenderWindow* pRenderWindow);
    void destroyAllRenderWindows(void);

    RenderWindow* getPrimaryRenderWindow(void)
    {
        return m_PrimaryRenderWindow;
    }

    const Capabilities& getCapabilities(void) const;

private:
    RenderEngine();
    ~RenderEngine();

    void createPrimaryWindow(void);
    void initGLExtensions(void);

    void initializeSubsystems(void);
    void shutdownSubsystems(void);
    void fireRenderSystemInitialized(void);

    void checkOpenGLVersion(void);
    void checkShaderCapability(void);

    void captureCapabilities(void);

private:
    bool m_Initialized;

    RenderWindowFactoryPtr m_pRenderWindowFactory;
    RenderWindow* m_PrimaryRenderWindow;
    std::set<RenderWindow*> m_RenderWindowSet;
    std::set<Listener*> m_ListenerSet;

    mutable mf::Mutex m_Mutex;

    Capabilities m_GLCapabilities;
};

} // namespace glvr
} // namespace surveon

#endif // GLVR_RENDER_ENGINE_H

