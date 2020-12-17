
#ifndef GLVR_X11_WINDOW_MANAGER_H
#define GLVR_X11_WINDOW_MANAGER_H

#include "GLVRX11Prerequisites.h"


namespace surveon
{
namespace glvr
{

class X11WindowManager: public mf::Singleton<X11WindowManager>
{
    friend class Singleton<X11WindowManager>;

public:
    X11WindowManager();

    ~X11WindowManager();

    Display* openDisplay(void);

    void closeDisplay(Display* pDisplay);

private:
    Display* m_pDisplay;

    uint32 m_ReferenceCount;

};



} // namespace glvr
} // namespace surveon



#endif // GLVR_X11_WINDOW_MANAGER_H
