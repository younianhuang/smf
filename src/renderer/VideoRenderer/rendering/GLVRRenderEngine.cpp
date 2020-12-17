
#include "GLVRRenderEngine.h"


namespace surveon
{
namespace glvr
{

RenderEngine::RenderEngine() :
    m_Initialized(false),
    m_PrimaryRenderWindow(nullptr)
{

}

RenderEngine::~RenderEngine()
{
    shutdown();
}

void RenderEngine::initialize(void)
{
    if(!m_Initialized)
    {
        GLVR_LOG_INFO("Initialize Rendering Engine.");

        m_pRenderWindowFactory = createRenderWindowFactory();

        createPrimaryWindow();

        captureCapabilities();

        initializeSubsystems();

        GLVR_LOG_INFO("Rendering System is initialized.");

        m_Initialized = true;

    }
}

RenderWindow* RenderEngine::createRenderWindow(mf::WindowHandle handle)
{
    RenderWindow* pRenderWindow = m_pRenderWindowFactory->create(handle);
    pRenderWindow->resetGLState();

    m_RenderWindowSet.insert(pRenderWindow);

    return pRenderWindow;
}

void RenderEngine::destroyRenderWindow(RenderWindow* pRenderWindow)
{    
    if(pRenderWindow == nullptr)
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Specified pointer is null!");
    }

    if(!mf::ContainerUtil::destroySetElement(m_RenderWindowSet, pRenderWindow))
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Specified pointer does not exist!");
    }
}

void RenderEngine::destroyAllRenderWindows(void)
{    
    mf::ContainerUtil::destroyAllElements(m_RenderWindowSet);
}

void RenderEngine::shutdown(void)
{
    if(m_Initialized)
    {
        GLVR_LOG_INFO("Shutdown Rendering Engine.");

        destroyAllRenderWindows();

        // NOTE:
        // OpenGL command must called before all OpenGL Context is destroyed.
        // So shut subsystem here.
        shutdownSubsystems();

        GLVR_LOG_INFO("detroy Primary Render Window.");        

        delete m_PrimaryRenderWindow;
        m_PrimaryRenderWindow = nullptr;

        m_Initialized = false;
    }
}

void RenderEngine::addListener(Listener* pListener)
{
    if(!pListener)
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Specified pointer is null!");
    }

    if(!m_ListenerSet.insert(pListener).second)
    {
        GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Specified pointer already exists!");
    }
}

void RenderEngine::removeListener(Listener* pListener)
{
    if(!m_ListenerSet.erase(pListener))
    {
         GLVR_THROW_EXCEPT(GLVRInvalidParametersException, "Specified pointer does not exist!");
    }
}

void RenderEngine::fireRenderSystemInitialized(void)
{
    for(auto p : m_ListenerSet)
    {
        p->onRenderSystemInitialized();
    }
}

void RenderEngine::checkOpenGLVersion(void)
{
    String glVersion = m_GLCapabilities.glVersion;

    if(!glVersion.empty())
    {
        double ver = atof(glVersion.c_str()); // Convert string to double

        if(ver >= 3.00)
        {
            GLVR_LOG_INFO("OpenGL support verison:" <<glVersion);
        }
        else
        {
            GLVR_THROW_EXCEPT(GLVROpenGLErrorException,
                              "Current OpenGL version " <<glVersion <<
                              " is not legal, Need 3.0.0 or later version");
        }
    }
    else
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to get OpenGL verison.");
    }
}

void RenderEngine::checkShaderCapability(void)
{
    String shaderVerison = m_GLCapabilities.shaderVersion;

    if(!shaderVerison.empty())
    {
        double ver = atof(shaderVerison.c_str()); // Convert string to double

        if(ver >= 1.30)
        {
            GLVR_LOG_INFO("OpenGL support shading language verison:" <<shaderVerison);
        }
        else
        {
            GLVR_THROW_EXCEPT(GLVROpenGLErrorException,
                              "Current Shader version " <<shaderVerison <<
                              " is not legal, Need 1.30 or later version");
        }
    }
    else
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail to get shading language verison.");
    }
}

const Capabilities& RenderEngine::getCapabilities(void) const
{
    return m_GLCapabilities;
}

void RenderEngine::captureCapabilities()
{
    // OpenGL Version
    String result((char *)glGetString(GL_VERSION));

      // strip strings after the first space
    size_t pos = result.find_first_of(' ');
    if (pos != String::npos)
        result.assign(result.c_str(), pos);

    m_GLCapabilities.glVersion = result;

    // Shader Version
    result = String((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

      // strip strings after the first space
    pos = result.find_first_of(' ');
    if (pos != String::npos)
        result.assign(result.c_str(), pos);

    m_GLCapabilities.shaderVersion = result;

    // GPU Model
    m_GLCapabilities.gpuModel = String((char *)glGetString(GL_RENDERER));
}

//=======================================================================================
void RenderEngine::createPrimaryWindow(void)
{    
    m_PrimaryRenderWindow = m_pRenderWindowFactory->create(1, 1, "GLVR Primary Window");

    m_PrimaryRenderWindow->setPrimary(true);

    initGLExtensions();

    m_PrimaryRenderWindow->resetGLState();
}

void RenderEngine::initGLExtensions(void)
{
    GLenum status = glewInit();
    if (status != GLEW_OK)
    {
        GLVR_THROW_EXCEPT(GLVROpenGLErrorException, "Fail init glew");
    }
}

void RenderEngine::initializeSubsystems()
{
    checkOpenGLVersion();
    checkShaderCapability();
    GPUProgramManager::getInstance().initialize();
}

void RenderEngine::shutdownSubsystems()
{    
    GPUBufferManager::destroyInstance();
    GPUProgramManager::destroyInstance();
    TextureManager::destroyInstance();
}


} // namespace glvr
} // namespace surveon

