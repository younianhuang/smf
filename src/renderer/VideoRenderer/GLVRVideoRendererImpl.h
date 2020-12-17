/*

*/

#ifndef SURVEON_GLVR_VIDEO_RENDERER_IMPL_H
#define SURVEON_GLVR_VIDEO_RENDERER_IMPL_H

#include "GLVRIVideoRenderer.h"
#include "GLVRVideoPaneImpl.h"
#include "GLVRRetangleShapeImpl.h"
#include "GLVRConvexShapeImpl.h"
#include "GLVRSpriteImpl.h"
#include <framework/SMFUtil.h>

namespace surveon
{

namespace glvr
{

class VideoRendererImpl : public IVideoRenderer
{

public:
        VideoRendererImpl(mf::AsyncCommandQueue::SyncControl* pSyncControl);
        virtual ~VideoRendererImpl(void);

        virtual void initialize(mf::WindowHandle handle);
        virtual void shutdown(void);
        virtual bool isInitialized(void) const;

        virtual IVideoPane* createVideoPane(void);
        virtual void destroyVideoPane(IVideoPane* pVideoPane);
        virtual void destroyAllVideoPanes(void);
        virtual void flush(void);

        virtual IRetangleShape* createRetangleShape(const mf::FloatRect& rect);
        virtual void destroyRetangleShape(IRetangleShape* pRetangleShape);
        virtual void destroyAllRetangleShapes(void);

        virtual IConvexShape* createConvexShape(uint32 pointCount);
        virtual void destroyConvexShape(IConvexShape*);
        virtual void destroyAllConvexShapes(void);

        virtual ISprite* createSprite(const mf::FloatRect& rect);
        virtual void destroySprite(ISprite*);
        virtual void destroyAllSprites(void);

        virtual void repaint(void);
        virtual void setViewport(const mf::FloatRect& rect);

        virtual Statistics getStatistics(void) const;

        virtual void setPresentationClock(mf::IPresentationClock* pPresentationClock);

        virtual mf::IVideoBuffer* createGPUHardwareVideoBuffer(uint16 width, uint16 height);

        virtual void destroyGPUHardwareVideoBuffer(mf::IVideoBuffer* pVideoBuffer);

        virtual IDeviceManager* getDeviceManager(void);

        void render(void);

        bool isUpdated(void) const;

        bool hasMoreCommand(void) const;

        micro_seconds predictDelayTime(void);

        void executeCommands(void);

private:
        void initializeCommand(mf::WindowHandle handle);
        void shutdownCommand(void);

        void destroyAllRenderablesCommand(void);

        IVideoPane* createVideoPaneCommand(void);
        void destroyVideoPaneCommand(IVideoPane* pVideoPane);
        void destroyAllVideoPanesCommand(void);
        void flushCommand(void);

        IRetangleShape* createRetangleShapeCommand(const mf::FloatRect& rect);
        IConvexShape* createConvexShapeCommand(uint32 pointCount);
        ISprite* createSpriteCommand(const mf::FloatRect& rect);

        void emptyCommand(void);
        void setViewportCommand(const mf::FloatRect& rect);

        const Statistics& getStatisticsCommand(void) const;

        void setPresentationClockCommand(mf::IPresentationClock* pPresentationClock);

        mf::IVideoBuffer* createPixmapVideoBufferCommand(uint16 width, uint16 height);
        void destroyPixmapVideoBufferCommand(mf::IVideoBuffer* pVideoBuffer);

        IDeviceManager* getDeviceManagerCommand(void);


        void resetStatistics(void);
        void updateStatistics(void);


private:
    SMF_ATOMIC<bool> m_Initialized;
    bool m_Updated;

    RenderWindow* m_pRenderWindow;

    std::set<VideoPaneImpl*> m_VideoPaneSet;

    typedef mf::HierarchyObjectFactory<IRetangleShape> RetangleShapeFactory;
    RetangleShapeFactory m_RetangleShapeFactory;

    typedef mf::HierarchyObjectFactory<IConvexShape> ConvexShapeFactory;
    ConvexShapeFactory m_ConvexShapeFactory;

    typedef mf::HierarchyObjectFactory<ISprite> SpriteFactory;
    SpriteFactory m_SpriteFactory;

    HierarchyObjectFactory<IVideoBuffer> m_HardwareVideoBufferFactory;

    mutable mf::AsyncCommandQueue m_CommandQueue;

    mutable mf::Mutex m_Mutex;

    mf::WindowHandle m_WindowHandle;

    mf::FramerateCounter m_LastFramerateCounter;
    mf::FramerateCounter m_AvgFramerateCounter;

    Statistics m_Statistics;

    mf::IPresentationClock* m_pPresentationClock;

    int m_Index;
    static int ms_Count;
};

} // namespace glvr
} // namespace surveon




#endif // SURVEON_GLVR_VIDEO_RENDERER_IMPL_H

