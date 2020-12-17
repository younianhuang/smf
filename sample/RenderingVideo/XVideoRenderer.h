#ifndef XVIDEORENDERER_H
#define XVIDEORENDERER_H

#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)

// to avoid compile error
#include <QtCore/qcoreevent.h>
#include <QtWidgets/qstyleoption.h>
#include <QtCore/qurl.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xvlib.h>
#include <QtX11Extras/QX11Info>

#include <framework/SMFUtil.h>

class XVideoRenderer
{
public:
    XVideoRenderer(Drawable d);
    ~XVideoRenderer();

    void initClock();
    void processVideo(char *data, int width, int height,
                      int src_x, int src_y, unsigned int src_w, unsigned int src_h,
                      int dest_x, int dest_y, unsigned int dest_w, unsigned int dest_h);
    float getFPS() const;

protected:
    void getDisplay();
    void getPort();
    bool findYV12Port(Display* display, XvPortID port);
    void createImage(char *data, int width, int height);
    void createGC();
    void putImage(int src_x, int src_y, unsigned int src_w, unsigned int src_h,
                  int dest_x, int dest_y, unsigned int dest_w, unsigned int dest_h);

private:
    surveon::mf::Clock m_Clock;
    Display* m_pDisplay;
    XvPortID m_Port;
    int m_FormatID;
    XvImage* m_pImage;
    GC m_GC;
    surveon::mf::FramerateCounter m_FramerateCounter;
    std::atomic<float> m_FPS;
    Drawable m_WinID;
};

#endif // PROCESS_VIDEO_CONF

#endif // XVIDEORENDERER_H
