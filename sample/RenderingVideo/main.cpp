#include "RenderingVideoWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <CommonConfig.h>

int main(int argc, char *argv[])
{
    surveon::glmark::Log::setup("glmark", "glmark.log");

#if ((PROCESS_VIDEO_CONF == PROCESS_VIDEO_GLVR) && defined(SMF_PLATFORM_LINUX))
    setenv("vblank_mode", "0", 1); // disable vsync
#endif // PROCESS_VIDEO_CONF

    QApplication a(argc, argv);
    RenderingVideoWindow w;

    try
    {
        w.show();

        return a.exec();
    }
    catch(std::exception& ex)
    {
        QMessageBox msgBox;

        QString msg = QString("This application stop caused by error:\n %1").
                arg(ex.what());

        msgBox.setText(msg);

        msgBox.exec();

        return -1;
    }

}
