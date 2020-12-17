#include "CommonConfig.h"

#if (PROCESS_VIDEO_CONF == PROCESS_VIDEO_XVR)

#include <iostream>

XVideoRenderer::XVideoRenderer(Drawable d) :
    m_pDisplay(nullptr),
    m_FormatID(0),
    m_pImage(nullptr),
    m_FramerateCounter(3000),
    m_WinID(d)
{
    getDisplay();
    getPort();
    createGC();
}

XVideoRenderer::~XVideoRenderer()
{
    free(m_pDisplay);
    free(m_pImage);
}

//===============================================================================================

void XVideoRenderer::initClock()
{
    m_Clock.restart();
}

void XVideoRenderer::processVideo(char *data, int width, int height,
                                  int src_x, int src_y, unsigned int src_w, unsigned int src_h,
                                  int dest_x, int dest_y, unsigned int dest_w, unsigned int dest_h)
{
    // create image
    createImage(data, width, height);

    // put image
    putImage(src_x, src_y, src_w, src_h,
             dest_x, dest_y, dest_w, dest_h);

    // free image
    XFlush(m_pDisplay);
    XFree(m_pImage);

    // update fps
    m_FramerateCounter.update(m_Clock.getMilliseconds());
    if(m_FPS != m_FramerateCounter.getFPS())
    {
        m_FPS = m_FramerateCounter.getFPS();
    }
}

float XVideoRenderer::getFPS() const
{
    return m_FPS;
}

//===============================================================================================

void XVideoRenderer::getDisplay()
{
    m_pDisplay = QX11Info::display();
}

void XVideoRenderer::getPort()
{
    unsigned int adaptor_num;
    XvAdaptorInfo *adaptor_info = NULL;

    if (XvQueryAdaptors(m_pDisplay, DefaultRootWindow(m_pDisplay), &adaptor_num, &adaptor_info) != Success)
        std::cout << "XvQueryAdaptors failed." << std::endl;

    for(int i=0; i<adaptor_num; i++)
    {
        for(int j=0; j<adaptor_info[i].num_ports; j++)
        {
            if (findYV12Port(m_pDisplay, adaptor_info[i].base_id+j))
            {
                m_Port = adaptor_info[i].base_id + j;
                break;
            }
        }
    }

    if(adaptor_info)
        XvFreeAdaptorInfo(adaptor_info);
}

bool XVideoRenderer::findYV12Port(Display* display, XvPortID port)
{
    int count;
    XvImageFormatValues *formatValues;
    formatValues = XvListImageFormats(display, port, &count);
    for (int i=0; i<count; i++)
    {
        if (formatValues[i].type==XvYUV && strcmp(formatValues[i].guid,"YV12")==0)
        {
            if (XvGrabPort(display, port, 0) == Success)
            {
                m_FormatID = formatValues[i].id;
                XFree(formatValues);
                return true;
            }
        }
    }

    XFree(formatValues);
    return false;
}

void XVideoRenderer::createImage(char *data, int width, int height)
{
    m_pImage = XvCreateImage(m_pDisplay, m_Port, m_FormatID, data, width, height);

    if (m_pImage == NULL)
        std::cout << "XvCreateImage failed." << std::endl;
}

void XVideoRenderer::createGC()
{
    m_GC = XCreateGC(m_pDisplay, m_WinID, 0, 0);

    if (!m_GC)
        std::cout << "XCreateGC failed." << std::endl;
}

void XVideoRenderer::putImage(int src_x, int src_y, unsigned int src_w, unsigned int src_h,
                              int dest_x, int dest_y, unsigned int dest_w, unsigned int dest_h)
{
    XvPutImage(m_pDisplay, m_Port, m_WinID, m_GC, m_pImage, src_x, src_y, src_w, src_h,
                                                            dest_x, dest_y, dest_w, dest_h);
}

#endif // PROCESS_VIDEO_CONF
