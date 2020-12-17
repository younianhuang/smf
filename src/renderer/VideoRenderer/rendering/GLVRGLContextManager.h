#ifndef GLVR_GLCONTEXT_MANAGER_H
#define GLVR_GLCONTEXT_MANAGER_H

#include "GLVRRenderEnginePrerequisites.h"
#include "GLVRGLContext.h"

namespace surveon
{
namespace glvr
{

template<class GLContextType>
class GLContextManager : public Singleton<GLContextManager<GLContextType>>
{
    friend class Singleton<GLContextManager<GLContextType>>;

public:
    template<class... Args>
    GLContextType* createGLContext(Args&&... arg)
    {
        if(!m_pPrimaryContext)
        {
            m_pPrimaryContext = m_GLContextFactory.create(arg...);
            return m_pPrimaryContext;
        }
        else
        {
            return m_GLContextFactory.create(m_pPrimaryContext, arg...);
        }
    }

    void destroyGLContext(GLContextType* pGLContext)
    {
        assert(pGLContext);

        if(m_pCurrentContext == pGLContext)
        {

            if(m_pPrimaryContext == pGLContext)
            {
                //NOTE:
                // primary context must be last one to be destroyed
                assert(m_GLContextFactory.getNumOfItems() == 1);
                m_pPrimaryContext = nullptr;
                makeCurrent(nullptr);
            }
            else
            {
                makeCurrent(m_pPrimaryContext);
            }            
        }

        m_GLContextFactory.destroy(pGLContext);

    }


    void makeCurrent(GLContextType* pContext)
    {
        if(pContext)
        {
            if(m_pCurrentContext != pContext)
            {
                pContext->makeCurrent();
                m_pCurrentContext = pContext;
            }
        }
        else
        {
            if(m_pCurrentContext)
            {
                m_pCurrentContext->makeCurrent(false);
                m_pCurrentContext = nullptr;
            }
        }
    }



private:
        GLContextManager(void) :
            m_pPrimaryContext(nullptr),
            m_pCurrentContext(nullptr)
        {}

private:
    GLContextType* m_pPrimaryContext;
    GLContextType* m_pCurrentContext;

    mf::SimpleObjectFactory<GLContextType> m_GLContextFactory;
};


} // namespace glvr
} // namespace surveon



#endif // GLVR_GLCONTEXT_MANAGER_H




