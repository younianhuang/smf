#include "GLMark.h"
#include "ViewPane.h"
#include <CommonConfig.h>

int main(int argc, char *argv[])
{
    surveon::glmark::Log::setup("glmark", "glmark.log");

#ifdef SMF_PLATFORM_LINUX
    setenv("vblank_mode", "0", 1); // disable vsync
#endif // SMF_PLATFORM_LINUX

    QApplication a(argc, argv);

    QCommandLineParser parser;

    // An option with a value
    QCommandLineOption exeRenderingVideoWindowOption("s", "RenderingVideoWindow Mode", "Screen Number");
    parser.addOption(exeRenderingVideoWindowOption); // Accept Command Like: "GLMark -s 0"

    // A boolean option with a single name (-a)
    QCommandLineOption exeRunAllOption("a", "Run All");
    parser.addOption(exeRunAllOption); // Accept Command Like: "GLMark -a"

    // Process the actual command line arguments given by the user
    parser.process(a);
    bool exeRenderingVideoWindow = parser.isSet(exeRenderingVideoWindowOption);
    bool exeRunAll = parser.isSet(exeRunAllOption);

    if (exeRenderingVideoWindow) // RenderingVideoWindow Mode
    {
        // Get Screen Number from Parent Process to Calculate Left Position
        const int screenNumber = parser.value(exeRenderingVideoWindowOption).toInt();
        GLMARK_LOG_INFO("Execute './GLMark -s " << screenNumber << "'");

        int left = 0;
        for (int i=0; i<screenNumber; i++)
        {
            left += QApplication::desktop()->screenGeometry(i).width();
        }

        QString description(QString("Multiple Processes Screen %1").arg(screenNumber));
        RenderingVideoWindow w(NO_PARENT, 4, 4, surveon::mf::SMF_VIDEO_FORMAT_YV12, ViewPane::RES_1080p,
                               QDir::currentPath(), QDir::currentPath(),
                               HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                               SHOW_PROCESS_ID, description);
        description = "GLMark - " + description + " (Press ESC to quit)";
        w.setWindowTitle(description);

        // Set to Right Position and Full Size
        const QRect screenGeometry = QApplication::desktop()->screenGeometry(screenNumber);
        w.setGeometry(left, 0, screenGeometry.width(), screenGeometry.height());
        w.move(left, 0);

        try
        {
            w.showMaximized();

            QTimer::singleShot(EXPORT_TIME, &w, SLOT(exportReport()));
            QTimer::singleShot(CLOSE_TIME, &w, SLOT(close()));

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
    else // GLMark Mode
    {
        GLMark w;
        w.setGeometry(200, 200, 800, 600);

        try
        {
            w.show();

            if (exeRunAll)
            {
                GLMARK_LOG_INFO("Execute './GLMark -a'");
                return w.exeRunAll();
            }

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
}
