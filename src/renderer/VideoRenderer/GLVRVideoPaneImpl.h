/*

*/
#ifndef GLVR_VIDEO_PANE_IMPL_H
#define GLVR_VIDEO_PANE_IMPL_H

#include "GLVRImplPrerequisites.h"
#include "GLVRIVideoPane.h"

namespace surveon
{
namespace glvr
{

class VideoPaneImpl : public IVideoPane
{
public:
    VideoPaneImpl(RenderTarget* pRenderTarget, mf::AsyncCommandQueue* pCommandQueue);

    virtual ~VideoPaneImpl(void);

    virtual void processData(mf::IVideoBuffer* pVideoBuffer);

    virtual void shareStreamFrom(IVideoPane* pVideoPane);

    virtual void stopShareStream(void);

    virtual void setSourceRect(const mf::FloatRect& normalizedRect);

    virtual mf::FloatRect getSourceRect(void) const;

    virtual void setDestinationRect(const mf::FloatRect& rect);

    virtual mf::FloatRect getDestinationRect(void) const;

    virtual void flush(void);

    void flushCommnad(void);

    virtual void setColor(const Color& color);

    virtual uint8 getZOrder(void) const;

    virtual void setZOrder(uint8 zorder);

    virtual void show(void);

    virtual void hide(void);

    virtual bool isShow(void) const;

    virtual void setBorderColor(const Color& color);

    virtual void setBorderThickness(int thickness);

    virtual mf::FloatRect getBoundaryRect(void) const;

    bool hasMoreData(void) const
    {
        return m_VideoBufferQueue.hasMore();
    }

    mf::IVideoBuffer* peekVideoData(void)
    {
        return m_VideoBufferQueue.peek();
    }

    void processQueuedDataCommand(void);

private:

        void initialize(void);

        void setSourceRectCommand(const mf::FloatRect& normalizedRect);

        mf::FloatRect getSourceRectCommand(void) const;

        void setDestinationRectCommand(const mf::FloatRect& rect);

        mf::FloatRect getDestinationRectCommand(void) const;

        void setZOrderCommand(uint8 zorder);

        uint8 getZOrderCommand(void) const;

        void shareStreamFromCommand(VideoPaneImpl* pVideoPane);

        void stopShareStreamCommand(void);

        MaterialSharedPtr createMaterial(mf::IVideoBuffer* pVideoBuffer);

        void setMaterial(MaterialSharedPtr pMaterial);

        void uploadVideoData(mf::IVideoBuffer* pVideoBuffer);

        void addGuest(VideoPaneImpl* pVideoPane)
        {
            m_GuestVideoPanes.insert(pVideoPane);
        }

        void removeGuest(VideoPaneImpl* pVideoPane)
        {
            m_GuestVideoPanes.erase(pVideoPane);
        }

private:
    RenderTarget* m_pRenderTarget;

    mf::AsyncCommandQueue* m_pCommandQueue;

    typedef mf::Queue<mf::IVideoBuffer*> VideoBufferQueue;
    mf::Queue<mf::IVideoBuffer*> m_VideoBufferQueue;

    mf::IVideoBuffer* m_pCurrentProcessedVideoBuffer;

    RetangleShape *  m_pRetangleShape;

    MaterialSharedPtr m_pMaterial;

    VideoPaneImpl* m_pHostVideoPane;
    std::set<VideoPaneImpl*> m_GuestVideoPanes;

    mf::MediaSubType m_VideoFormat;
    int m_VideoWidth;
    int m_VideoHeight;
};


} // namespace glvr
} // namespace surveon




#endif // GLVR_VIDEO_PANE_H

