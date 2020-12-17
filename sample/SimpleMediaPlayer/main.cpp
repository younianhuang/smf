
#include "SimpleMediaPlayerWindow.h"
#include <QApplication>
#include <CommonConfig.h>

int main(int argc, char *argv[])
{
    try
    {
        surveon::glmark::Log::setup("glmark", "glmark.log");

    #ifdef SMF_PLATFORM_LINUX
        setenv("vblank_mode", "0", 1); // disable vsync
    #endif

        QApplication a(argc, argv);
        QApplication::setApplicationName("SimpleMediaPlayer");
        QApplication::setApplicationVersion("1.0");

        QCommandLineParser parser;
        parser.setApplicationDescription("A simple media player which contains video decoding (support hardware decoding), video rendering, etc.");
        parser.addHelpOption(); // accept command: "SimpleMediaPlayer -h"
        parser.addVersionOption(); // accept command: "SimpleMediaPlayer -v"
        parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open.")); // accept command: "SimpleMediaPlayer ../resource/test1080p.avi"

        // A boolean option with a single name (--nr)
        QCommandLineOption exeNoRenderingOption("nr", QCoreApplication::translate("main", "No rendering mode. Only decoding the video file."));
        parser.addOption(exeNoRenderingOption); // accept command: "SimpleMediaPlayer --nr"

        // A boolean option with a single name (--nt)
        QCommandLineOption exeNoTimingControlOption("nt", QCoreApplication::translate("main", "No timing control."));
        parser.addOption(exeNoTimingControlOption); // accept command: "SimpleMediaPlayer --nt"

        // A boolean option with a single name (--hwdec)
        QCommandLineOption exeHardwareDecodeOption("hwdec", QCoreApplication::translate("main", "Enable hardware decoding."));
        parser.addOption(exeHardwareDecodeOption); // accept command: "SimpleMediaPlayer --hwdec"

        parser.process(a);
        bool exeNoRendering = parser.isSet(exeNoRenderingOption);
        bool exeNoTimingControl = parser.isSet(exeNoTimingControlOption);
        bool exeHardwareDecode = parser.isSet(exeHardwareDecodeOption);

        bool empty = parser.positionalArguments().isEmpty();
        QString filename;
        if(!empty)
        {
            QFileInfo fileInfo(parser.positionalArguments().at(0));
            filename = fileInfo.absoluteFilePath();
            GLMARK_LOG_INFO("Execute './SimpleMediaPlayer " << filename << "'");

            if(!fileInfo.exists())
            {
                QString msg = "File " + filename + " does not exist.\nAborted.";
                GLMARK_LOG_INFO(msg);
                std::cout << msg << std::endl;
                return 1;
            }
        }

        SimpleMediaPlayerWindow w;

        if(exeNoRendering)
        {
            w.disableRenderingVideo();
        }

        if(exeHardwareDecode)
        {
            w.enableHardwareDecode();
        }

        if(!empty)
        {
            w.setFileSource(filename);
            w.play();
        }

        if(exeNoTimingControl)
        {
            w.disableTimingControl();
        }

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
