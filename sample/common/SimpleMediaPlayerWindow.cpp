#include "SimpleMediaPlayerWindow.h"
#include "ui_SimpleMediaPlayerWindow.h"
#include <QInputDialog>
#include "Screenshot.h"

static bool flagPlugin = false;

using namespace surveon;

SimpleMediaPlayerWindow::SimpleMediaPlayerWindow(QWidget *parent, size_t row, size_t column,
                                                 bool renderingVideo, bool decodeVideo,
                                                 bool downScaling, int numberOfThread,
                                                 bool timingControl, float rate,
                                                 QString exportReportPath, QString saveScreenshotPath,
                                                 QString description) :
    QMainWindow(parent),
    ui(new Ui::SimpleMediaPlayerWindow),
    m_RenderingVideo(renderingVideo),
    m_DecodeVideo(decodeVideo),
    m_DownScaling(downScaling),
    m_TimingControl(timingControl),
    m_NumberOfThread(numberOfThread),
    m_Column(column),
    m_Row(row),
    m_TimeCounter(0),
    m_TotalTime(0),
    m_Rate(rate),
    m_SumCPU(0.0),
    m_Last10sCPU(0.0),
    m_SumRendererFPS(0.0),
    m_SumDecoderFPS(0.0),
    m_SumSourceFPS(0.0),
    m_Last10sDecoderFPS(0.0),
    m_Last10sSourceFPS(0.0),
    m_SumDecoderBandwidth(0.0),
    m_SumSourceBandwidth(0.0),
    m_Last10sDecoderBandwidth(0.0),
    m_Last10sSourceBandwidth(0.0),
    m_pMsgBar(new QLabel),
    m_pTimer(new QTimer(this)),
    m_Description(description),
    m_ExportReportPath(exportReportPath),
    m_SaveScreenshotPath(saveScreenshotPath),
    m_FileSource(QString()),
    m_Play(false),
    m_Stop(false),
    m_HardwareDecode(false)
{
    GLMARK_LOG_INFO("Create SimpleMediaPlayerWindow " << SMF_ADDR_PREFIX(this));

    mf::MediaManager* pMediaManager = mf::MediaManager::getInstancePtr();
    pMediaManager->initialize();

    if (flagPlugin == false)
    {
    // load plugins           
#ifdef NDEBUG
    pMediaManager->loadPlugin("ffmpegPlugin");
    //pMediaManager->loadPlugin("LIVE555Plugin");
#else    
    pMediaManager->loadPlugin("ffmpegPlugin-d");
    //pMediaManager->loadPlugin("LIVE555Plugin-d");
#endif
        flagPlugin = true;
    }

    setNumberOfThread();

    glvr::VideoRendererManager::getInstancePtr()->initialize();
    m_ViewPane.getGLInfo(); // MUST after VideoRendererManager initialization

    ui->setupUi(this);
    ui->statusBar->addPermanentWidget(m_pMsgBar, true);
    setStyleSheet("QStatusBar::item { border: 0px; }");
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateStatistics()));

    createViewTiles();

    if (renderingVideo != ui->actionRendering_Video->isChecked())
    {
        ui->actionRendering_Video->setChecked(renderingVideo);
        ui->actionRendering_Video->triggered(renderingVideo);
    }

    if (decodeVideo != ui->actionDecode_Video->isChecked())
    {
        ui->actionDecode_Video->setChecked(decodeVideo);
        ui->actionDecode_Video->triggered(decodeVideo);
    }

    if (downScaling != ui->actionDown_Scaling->isChecked())
    {
        SLEEP(1);
        ui->actionDown_Scaling->setChecked(downScaling);
        ui->actionDown_Scaling->triggered(downScaling);
    }

    if (timingControl != ui->actionTimingControl->isChecked())
    {
        ui->actionTimingControl->setChecked(timingControl);
        ui->actionTimingControl->triggered(timingControl);
    }

    setWindowTitle(description);

}

SimpleMediaPlayerWindow::~SimpleMediaPlayerWindow()
{
    delete ui;

    GLMARK_LOG_INFO("Destroy SimpleMediaPlayerWindow");

    mf::MediaManager::getInstancePtr()->unloadAllPlugins();

    glvr::VideoRendererManager::getInstancePtr()->shutdown();

    mf::MediaManager::getInstancePtr()->shutdown();
}

void SimpleMediaPlayerWindow::resizeEvent (QResizeEvent * event)
{
    setupLayout();

    QMainWindow::resizeEvent(event);
}

void SimpleMediaPlayerWindow::closeEvent ( QCloseEvent * event )
{
    if (m_Play)
        closeFile();

    for (ViewTiles::iterator it=m_ViewTiles.begin(); it!=m_ViewTiles.end(); ++it)
    {
        delete *it;
    }

    ALL_LABELS close();

    emit sendClosed();

    GLMARK_LOG_INFO("Close SimpleMediaPlayerWindow");

    QMainWindow::closeEvent(event);
}

void SimpleMediaPlayerWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        GLMARK_LOG_INFO("Press key 'Esc'");
        setUserTerminate();
    }
}

void SimpleMediaPlayerWindow::setUserTerminate()
{
    emit sendUserTerminate();

    this->close();
}

void SimpleMediaPlayerWindow::on_actionChange_Layout_triggered()
{
    QWidget* popup = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    popup->setAttribute(Qt::WA_DeleteOnClose);

    QLabel *infoRow = new QLabel("Row(1~8):");
    QLineEdit* askRow = new QLineEdit(QString("%1").arg(m_Row));
    QLabel *infoColumn = new QLabel("Column(1~8):");
    QLineEdit* askColumn = new QLineEdit(QString("%1").arg(m_Column));
    QPushButton *buttonOK = new QPushButton("OK");
    m_pAskRow = askRow;
    m_pAskColumn = askColumn;

    connect(buttonOK, SIGNAL(clicked()), popup, SLOT(close()));
    connect(buttonOK, SIGNAL(clicked()), this, SLOT(validateSize()));

    layout->addWidget(infoRow);
    layout->addWidget(askRow);
    layout->addWidget(infoColumn);
    layout->addWidget(askColumn);
    layout->addWidget(buttonOK);
    popup->setLayout(layout);
    popup->setWindowTitle("Size");
    popup->show();
}

void SimpleMediaPlayerWindow::updateStatistics()
{
    m_TimeCounter++;

    if (m_ViewTiles.empty())
        return;

    QString message("  ");

    // cpu
    if (ui->actionCPU->isChecked())
    {
        float tmpCPU = m_ViewPane.getProcessCPU();
        message += QString("CPU: %1% / %2%   ").arg(tmpCPU, 0, 'f', 2).arg(m_ViewPane.getTotalCPU(), 0, 'f', 2);
        m_SumCPU += tmpCPU;

        if ((m_TimeCounter > m_TotalTime - STATISTICS_TIME) &&
            (m_TimeCounter <= m_TotalTime))
            m_Last10sCPU += tmpCPU;
    }

    // virtual memory
    if (ui->actionMemory->isChecked())
    {
        size_t memoryUsed = m_ViewPane.getMemoryUsed();
        message += QString("Memory: %1M   ").arg(memoryUsed);
    }

    // fps
    if (ui->actionFPS->isChecked())
    {
        float totalRendererFPS = 0.0;
        float totalDecoderFPS = 0.0;
        size_t totalDecoderBitrate = 0;
        float totalSourceFPS = 0.0;
        size_t totalSourceBitrate = 0;

        Labels::iterator itLabel = m_Labels.begin();
        for(ViewTiles::iterator it=m_ViewTiles.begin(); it!=m_ViewTiles.end(); ++it)
        {
            float tmpRendererFPS = ui->actionRendering_Video->isChecked() &&
                                   ui->actionDecode_Video->isChecked() ? (*it)->getRendererFPS() : 0;
            float tmpDecoderFPS = ui->actionDecode_Video->isChecked() ? (*it)->getDecoderFPS() : 0;
            size_t tmpDecoderBitrate = ui->actionDecode_Video->isChecked() ? (*it)->getDecoderBitrate() : 0;
            float tmpSourceFPS = (*it)->getSourceFPS();
            size_t tmpSourceBitrate = (*it)->getSourceBitrate();

            QString fps;
            fps += QString("  Renderer FPS: %1\n").arg(tmpRendererFPS, 0, 'f', 2); // float to QString
            fps += QString("  Decoder FPS: %1   ").arg(tmpDecoderFPS, 0, 'f', 2); // float to QString
            fps += QString("  Decoder Bitrate: %1 kb/s\n").arg(tmpDecoderBitrate >> 10);
            fps += QString("  Source FPS: %1   ").arg(tmpSourceFPS, 0, 'f', 2); // float to QString
            fps += QString("  Source Bitrate: %1 kb/s").arg(tmpSourceBitrate >> 10);
            (*itLabel++)->setText(fps);

            totalRendererFPS += tmpRendererFPS;
            totalDecoderFPS += tmpDecoderFPS;
            totalDecoderBitrate += tmpDecoderBitrate;
            totalSourceFPS += tmpSourceFPS;
            totalSourceBitrate += tmpSourceBitrate;
        }

        message += QString("RFPS: %1   ").arg(totalRendererFPS, 0, 'f', 2); // float to QString
        message += QString("DFPS: %1   ").arg(totalDecoderFPS, 0, 'f', 2); // float to QString
        message += QString("DBR: %1 kb/s   ").arg(totalDecoderBitrate >> 10);
        message += QString("SFPS: %1   ").arg(totalSourceFPS, 0, 'f', 2); // float to QString
        message += QString("SBR: %1 kb/s   ").arg(totalSourceBitrate >> 10);

        m_SumRendererFPS += totalRendererFPS;
        m_SumDecoderFPS += totalDecoderFPS;
        m_SumSourceFPS += totalSourceFPS;
        m_SumDecoderBandwidth += totalDecoderBitrate >> 10;
        m_SumSourceBandwidth += totalSourceBitrate >> 10;

        if ((m_TimeCounter > m_TotalTime - STATISTICS_TIME) &&
            (m_TimeCounter <= m_TotalTime))
        {
            m_Last10sRendererFPS += totalRendererFPS;
            m_Last10sDecoderFPS += totalDecoderFPS;
            m_Last10sSourceFPS += totalSourceFPS;
            m_Last10sDecoderBandwidth += totalDecoderBitrate >> 10;
            m_Last10sSourceBandwidth += totalSourceBitrate >> 10;
        }
    }

    m_pMsgBar->setText(message);

    // self control report & snapshot & close time
    if (m_TotalTime != 0) // benchmark mode
    {
        if (m_TimeCounter >= m_TotalTime + 3)
        {
            GLMARK_LOG_DEBUG("Timeout " << m_TotalTime + 3 << "s and close window");
            close();
        }
        else if (m_TimeCounter >= m_TotalTime + 2)
        {
            GLMARK_LOG_DEBUG("Timeout " << m_TotalTime + 2 << "s and save screenshot");
            quickshot();
        }
        else if (m_TimeCounter >= m_TotalTime + 1)
        {
            GLMARK_LOG_DEBUG("Timeout " << m_TotalTime + 1 << "s and export report");
            exportReport();
        }
    }
}

void SimpleMediaPlayerWindow::validateSize()
{
    size_t row = m_pAskRow->text().toULongLong();
    size_t column = m_pAskColumn->text().toULongLong();
    if (row>=1 && row<=8 && column>=1 && column<=8) // validate
    {
        if (m_Row!=row || m_Column!=column) // if changed
        {
            m_Row = row;
            m_Column = column;
            GLMARK_LOG_INFO("Change layout to " << m_Row << " x " << m_Column);
            restart();
        }
    }
    else
    {
        on_actionChange_Layout_triggered(); // input again
    }
}

void SimpleMediaPlayerWindow::createViewTiles()
{
    if(m_ViewTiles.empty())
    {
        size_t count = m_Column * m_Row;
        for(int i=0; i<count; i++ )
        {
            ViewTile* pViewTile = new ViewTile(this->centralWidget());
            m_ViewTiles.push_back(pViewTile);
            QLabel* pLabel = new QLabel(this);
            m_Labels.push_back(pLabel);
        }
    }
}

void SimpleMediaPlayerWindow::setupLayout()
{
    if (m_ViewTiles.empty())
        return;

    QRect displayArea = ui->centralWidget->geometry();

    int tileLeft = 0;
    int tileTop = 0;
    int tileWidth = (displayArea.width() - tileLeft* 2)/(int)m_Column;
    int labelHeight = 60;
    int tileHeight = (displayArea.height() - tileTop*2)/(int)m_Row - labelHeight;

    QRect tileRect(tileLeft, tileTop, tileWidth, tileHeight);

    ViewTiles::iterator it = m_ViewTiles.begin();
    Labels::iterator itLabel = m_Labels.begin();
    for(int j = 0; j < m_Row; j ++)
    {
        tileRect.moveTop(tileTop + j * tileHeight + labelHeight * (j+1));

        for(int i = 0; i < m_Column; i ++)
        {
            tileRect.moveLeft(tileLeft + i * tileWidth);
            (*it++)->setGeometry(tileRect);
            (*itLabel++)->setGeometry(tileRect.x(), tileRect.y()-labelHeight+ui->menuBar->height(),
                                      tileRect.width(), labelHeight);
        }
    }
}

void SimpleMediaPlayerWindow::restart()
{
    bool isPlaying = m_Play;

    if(m_Play)
    {
        closeFile();
    }

    for(ViewTiles::iterator it = m_ViewTiles.begin(); it!=m_ViewTiles.end(); ++it)
    {
        delete *it;
    }
    m_ViewTiles.clear(); // clear list

    for(Labels::iterator it = m_Labels.begin(); it!=m_Labels.end(); ++it)
    {
        (*it)->close(); // close individual windows
        delete *it;
    }
    m_Labels.clear(); // clear list

    createViewTiles();

    setupLayout();

    ALL_VIEWTILES show();

    ALL_LABELS show();

    reinit();

    if(isPlaying)
    {
        play();
    }
}

void SimpleMediaPlayerWindow::setNumberOfThread()
{
    mf::MediaManager* pMediaManager = mf::MediaManager::getInstancePtr();
    mf::Attributes* pAttributes = pMediaManager->getMediaTransformAttributes("ffmpegVideoDecoder");
    if(pAttributes)
    {
        pAttributes->setAttribuate(mf::ATTRI_NUMBER_OF_THREAD, m_NumberOfThread);
        GLMARK_LOG_DEBUG("Set number of thread = " << m_NumberOfThread);
    }
}

void SimpleMediaPlayerWindow::setRate()
{
    if (!m_Play)
        return;

    ALL_VIEWTILES setRate(m_Rate);
    GLMARK_LOG_DEBUG("Set rate = " << m_Rate);
}

void SimpleMediaPlayerWindow::on_actionThread_Number_triggered()
{
    int num = QInputDialog::getInt(this, "Thread Number",
                                   QString("Set Number of Thread(1~%1):").arg(m_ViewPane.getNumProcessor()),
                                   m_NumberOfThread, 1, m_ViewPane.getNumProcessor());

    if (m_NumberOfThread != num)
    {
        m_NumberOfThread = num;
        setNumberOfThread();
    }
}

void SimpleMediaPlayerWindow::on_actionSetRate_triggered()
{
    float rate = QInputDialog::getDouble(this, "Timing Control", "Set Rate:", m_Rate, 0.0f, FLOAT_MAX);

    if (m_Rate != rate)
    {
        m_Rate = rate;
        setRate();
    }
}

void SimpleMediaPlayerWindow::on_actionTimingControl_triggered(bool timingControl)
{
    if (m_Play)
    {
        for(ViewTiles::iterator it = m_ViewTiles.begin(); it!=m_ViewTiles.end(); ++it)
        {
            (*it)->setTimingControl(timingControl);
            if (timingControl)
                (*it)->setRate(m_Rate);
        }
        GLMARK_LOG_DEBUG("Set flag timing control = " << (timingControl ? "true" : "false"));
    }

    m_TimingControl = timingControl;
}

void SimpleMediaPlayerWindow::disableTimingControl(void)
{
    ALL_VIEWTILES setTimingControl(false);

    ui->actionTimingControl->setChecked(false);

    GLMARK_LOG_INFO("Disable timing control");
}

void SimpleMediaPlayerWindow::quickshot()
{
    Screenshot screenshot(m_ExportReportPath, m_SaveScreenshotPath);
    screenshot.quickshot(m_Description);

    GLMARK_LOG_INFO("Save screenshot in " << m_SaveScreenshotPath);
}

void SimpleMediaPlayerWindow::on_actionExportReport_triggered()
{
    exportReport(SHOW_MESSAGE_BOX);
}

void SimpleMediaPlayerWindow::on_actionScreenshot_triggered()
{
    quickshot();

    QMessageBox* pMsgBox = new QMessageBox;
    pMsgBox->setAttribute(Qt::WA_DeleteOnClose);
    pMsgBox->setWindowTitle("File Saved Successfully");
    pMsgBox->setText("Save screenshot in " + m_SaveScreenshotPath);
    pMsgBox->setButtonText(QMessageBox::Ok, "&Close");
    pMsgBox->show();

    QTimer::singleShot(3000, pMsgBox, SLOT(close()));
}

void SimpleMediaPlayerWindow::reinit()
{
    m_TimeCounter = 0;
    m_SumCPU = 0.0;
    m_Last10sCPU = 0.0;
    m_SumRendererFPS = 0.0;
    m_SumDecoderFPS = 0.0;
    m_SumSourceFPS = 0.0;
    m_Last10sRendererFPS = 0.0;
    m_Last10sDecoderFPS = 0.0;
    m_Last10sSourceFPS = 0.0;
    m_SumDecoderBandwidth = 0.0;
    m_SumSourceBandwidth = 0.0;
    m_Last10sDecoderBandwidth = 0.0;
    m_Last10sSourceBandwidth = 0.0;
}

void SimpleMediaPlayerWindow::exportReport(bool showMessageBox)
{
    QString description(m_Description);

    // Trim Spaces
    description.replace(" ", "");

    // get formatted current date time as file name, e.g., GLMark_Report_2015-05-29_16-22-38
    QString fileName(QString("GLMark_Report_%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss")));

    if (!m_Description.isEmpty())
    {
        fileName = QString("%1_%2").arg(fileName).arg(description);
    }

    std::ofstream ofs(QString("%1/%2.html").arg(m_ExportReportPath).arg(fileName).toStdString().c_str());
    ofs.precision(2);

    // write HTML file
    QString tmpTime = QString("%1").arg(showMessageBox ? m_TimeCounter : STATISTICS_TIME);
    QString tmpCPU = QString("%1").arg(showMessageBox ? m_SumCPU / m_TimeCounter : m_Last10sCPU / STATISTICS_TIME, 0, 'f', 2);
    QString strRenderingVideo = m_RenderingVideo ? "Enable" : "Disable";
    QString strDecodeVideo = m_DecodeVideo ? "Enable" : "Disable";
    QString strDownScaling = m_DownScaling ? "Enable" : "Disable";
    QString strTimingControl = m_TimingControl ? "Enable" : "Disable";
    QString strHardwareDecode = m_HardwareDecode ? "Enable" : "Disable";
    QString tmpMem = QString("%1").arg(m_ViewPane.getMemoryUsed());
    QString tmpTRFPS = QString("%1").arg(showMessageBox ? m_SumRendererFPS / m_TimeCounter : m_Last10sRendererFPS / STATISTICS_TIME, 0, 'f', 2);
    QString tmpRFPS = QString("%1").arg(showMessageBox ? m_SumRendererFPS / (m_TimeCounter * m_Row * m_Column) : m_Last10sRendererFPS / (STATISTICS_TIME * m_Row * m_Column), 0, 'f', 2);
    QString tmpTDFPS = QString("%1").arg(showMessageBox ? m_SumDecoderFPS / m_TimeCounter : m_Last10sDecoderFPS / STATISTICS_TIME, 0, 'f', 2);
    QString tmpDFPS = QString("%1").arg(showMessageBox ? m_SumDecoderFPS / (m_TimeCounter * m_Row * m_Column) : m_Last10sDecoderFPS / (STATISTICS_TIME * m_Row * m_Column), 0, 'f', 2);
    QString tmpTSFPS = QString("%1").arg(showMessageBox ? m_SumSourceFPS / m_TimeCounter : m_Last10sSourceFPS / STATISTICS_TIME, 0, 'f', 2);
    QString tmpSFPS = QString("%1").arg(showMessageBox ? m_SumSourceFPS / (m_TimeCounter * m_Row * m_Column) : m_Last10sSourceFPS / (STATISTICS_TIME * m_Row * m_Column), 0, 'f', 2);
    QString tmpTDBW = QString("%1").arg(showMessageBox ? m_SumDecoderBandwidth / m_TimeCounter : m_Last10sDecoderBandwidth / STATISTICS_TIME, 0, 'f', 2);
    QString tmpDBW = QString("%1").arg(showMessageBox ? m_SumDecoderBandwidth / (m_TimeCounter * m_Row * m_Column) : m_Last10sDecoderBandwidth / (STATISTICS_TIME * m_Row * m_Column), 0, 'f', 2);
    QString tmpTSBW = QString("%1").arg(showMessageBox ? m_SumSourceBandwidth / m_TimeCounter : m_Last10sSourceBandwidth / STATISTICS_TIME, 0, 'f', 2);
    QString tmpSBW = QString("%1").arg(showMessageBox ? m_SumSourceBandwidth / (m_TimeCounter * m_Row * m_Column) : m_Last10sSourceBandwidth / (STATISTICS_TIME * m_Row * m_Column), 0, 'f', 2);
    QString tmpWS = QString("%1x%2").arg(this->width()).arg(this->height());

    QString codec("No Target Window");
    if(!m_ViewTiles.empty())
    {
        ViewTiles::iterator it = m_ViewTiles.begin();
        codec = (*it)->getCodec();
    }

    QString videoResolution("Invalid");
    if(!m_ViewTiles.empty())
    {
        ViewTiles::iterator it = m_ViewTiles.begin();
        videoResolution = (*it)->getVideoResolution();
    }

    ofs << std::fixed << "                                                                     \
<!DOCTYPE html>                                                                                \
<html>                                                                                         \
                                                                                               \
    <head>                                                                                     \
                                                                                               \
        <title> GLMark Report</title>                                                          \
                                                                                               \
        <style>                                                                                \
            h1 {                                                                               \
                text-align: center;                                                            \
            }                                                                                  \
            table {                                                                            \
                border-collapse: collapse;                                                     \
                width: 60%;                                                                    \
                border: 1px solid #98bf21;                                                     \
            }                                                                                  \
            td {                                                                               \
                text-align: left;                                                              \
                border: 1px solid #98bf21;                                                     \
                padding: 5px;                                                                  \
                width: 50%;                                                                    \
            }                                                                                  \
            tr.alt td {                                                                        \
                background-color: #EAF2D3;                                                     \
                color: #000000;                                                                \
            }                                                                                  \
            caption {                                                                          \
                text-align: left;                                                              \
                font-size: 20px;                                                               \
            }                                                                                  \
        </style>                                                                               \
                                                                                               \
    </head>                                                                                    \
                                                                                               \
    <body>                                                                                     \
                                                                                               \
        <h1>GLMark Report - " << m_Description << "</h1>                                       \
                                                                                               \
        <p>                                                                                    \
            <table>                                                                            \
                <caption>System Information</caption> "
                << addHTMLTr(0, "Operating System", QString::fromStdString(m_ViewPane.getOperatingSystem()))
                << addHTMLTr(1, "OpenGL Version",   QString::fromStdString(m_ViewPane.getOpenGLVersion()))
                << addHTMLTr(0, "Shader Version",   QString::fromStdString(m_ViewPane.getShaderVersion()))
                << addHTMLTr(1, "CPU Model",        QString::fromStdString(m_ViewPane.getCPUModel()))
                << addHTMLTr(0, "GPU Model",        QString::fromStdString(m_ViewPane.getGPUModel())) << " \
            </table>                                                                           \
        </p>                                                                                   \
                                                                                               \
        <p>                                                                                    \
            <table>                                                                            \
                <caption>Configuration</caption> "
                << addHTMLTr(0, "Layout",          QString("%1x%2").arg(m_Row).arg(m_Column))
                << addHTMLTr(1, "Rendering Video", strRenderingVideo)
                << addHTMLTr(0, "Decode Video",    strDecodeVideo)
                << addHTMLTr(1, "Down Scaling",    strDownScaling)
                << QString("%1").arg(!m_HardwareDecode ? addHTMLTr(0, "Decoder Number Of Thread", QString("%1").arg(m_NumberOfThread)) : "")
                << addHTMLTr(!m_HardwareDecode, "Timing Control", strTimingControl)
                << addHTMLTr( m_HardwareDecode, "Rate", QString("%1").arg(m_Rate, 0, 'f', 2))
                << addHTMLTr(!m_HardwareDecode, "Hardware Decode", strHardwareDecode)
                << addHTMLTr( m_HardwareDecode, "Codec", codec)
                << addHTMLTr(!m_HardwareDecode, "Video Resolution", videoResolution)
                << addHTMLTr( m_HardwareDecode, "Window Size", tmpWS) << "                     \
            </table>                                                                           \
        </p>                                                                                   \
                                                                                               \
        <p>                                                                                    \
            <table>                                                                            \
                <caption>Statistics</caption> "
                << addHTMLTr(0, "Time",                            tmpTime + "s")
                << addHTMLTr(1, "Average CPU",                     tmpCPU +  "%")
                << addHTMLTr(0, "Virtual Memory Used",             tmpMem +  "M")
                << QString("%1").arg(m_RenderingVideo ? addHTMLTr(1, "Average Total Renderer FPS", tmpTRFPS) : "")
                << QString("%1").arg(m_RenderingVideo ? addHTMLTr(0, "Average Renderer FPS", tmpRFPS) : "")
                << addHTMLTr(1, "Average Total Decoder FPS",       tmpTDFPS)
                << addHTMLTr(0, "Average Decoder FPS",             tmpDFPS)
                << addHTMLTr(1, "Average Total Decoder Bandwidth", tmpTDBW + "kb/s")
                << addHTMLTr(0, "Average Decoder Bandwidth",       tmpDBW  + "kb/s")
                << addHTMLTr(1, "Average Total Source FPS",        tmpTSFPS)
                << addHTMLTr(0, "Average Source FPS",              tmpSFPS)
                << addHTMLTr(1, "Average Total Source Bandwidth",  tmpTSBW + "kb/s")
                << addHTMLTr(0, "Average Source Bandwidth",        tmpSBW  + "kb/s") << "      \
            </table>                                                                           \
        </p>                                                                                   \
    </body>                                                                                    \
</html>	                                                                                       \
    ";

    ofs.close();

    // success info
    if (showMessageBox)
    {
        QMessageBox* pMsgBox = new QMessageBox;
        pMsgBox->setAttribute(Qt::WA_DeleteOnClose);
        pMsgBox->setWindowTitle("File Saved Successfully");
        pMsgBox->setText("Export report in " + m_ExportReportPath);
        pMsgBox->setButtonText(QMessageBox::Ok, "&Close");
        pMsgBox->show();

        QTimer::singleShot(3000, pMsgBox, SLOT(close()));
    }

    GLMARK_LOG_INFO("Export report in " << m_ExportReportPath);
}

QString SimpleMediaPlayerWindow::addHTMLTr(bool alt, QString tag, QString text)
{
    QString tmp = alt ? " class='alt'" : "";
    return "<tr" + tmp + "><td>" + tag + "</td><td>" + text + "</td></tr>";
}

void SimpleMediaPlayerWindow::on_actionOpenFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "..//resource", "AVI Files (*.avi)");

    if (!filename.isEmpty())
    {
        if(m_Play)
        {
            closeFile();
        }

        setFileSource(filename);

        ui->action_Play->setEnabled(true);
        ui->action_Stop->setEnabled(true);
        ui->action_Pause->setEnabled(true);

        play();
    }
}

void SimpleMediaPlayerWindow::on_actionOpen_Network_Stream_triggered()
{
    QString streamPath = QInputDialog::getText(this, tr("Open Network Stream"),
                                              tr("Please enter a network URL:                 "), QLineEdit::Normal,
                                              tr("rtsp://172.26.12.55/stream1"));

    if(!streamPath.isEmpty())
    {
        if(m_Play)
        {
            closeFile();
        }

        setFileSource(streamPath);

        ui->action_Play->setEnabled(true);
        ui->action_Stop->setEnabled(true);
        ui->action_Pause->setEnabled(true);

        play();
    }
}

void SimpleMediaPlayerWindow::setTotalTime(size_t time)
{
    m_TotalTime = time;
    GLMARK_LOG_DEBUG("Set total time = " << m_TotalTime);
}

void SimpleMediaPlayerWindow::setFileSource(const QString& filename)
{
    if (!filename.isEmpty())
    {
        if(strncmp(filename.toStdString().c_str(), "rtsp", 4) == 0)
        {
            // Network stream
            m_FileSource = filename;

            ALL_VIEWTILES setFileSource( m_FileSource );
            GLMARK_LOG_INFO("Set file source = '" << m_FileSource << "'.");
        }
        else
        {
            // Local file
            QFileInfo fileInfo(filename);
            QString absoluteFilePath = fileInfo.absoluteFilePath();
            if (fileInfo.exists())
            {
                m_FileSource = absoluteFilePath;

                ALL_VIEWTILES setFileSource( m_FileSource );
                GLMARK_LOG_INFO("Set file source = '" << m_FileSource << "'.");
            }
            else
            {
                GLMARK_LOG_INFO("File '" << absoluteFilePath << "' does not exist. Set file source failed.");
            }
        }
    }
    else
    {
        GLMARK_LOG_INFO("Empty filename. Set file source failed.");
    }
}

void SimpleMediaPlayerWindow::on_action_Play_triggered()
{
    if(m_Stop) // state stop
    {
        ALL_VIEWTILES startSource();

        m_Stop = false;
    }
    else // state pause
    {
        ALL_VIEWTILES resumeSource();
    }

    ui->action_Play->setEnabled(false);
    ui->action_Stop->setEnabled(true);
    ui->action_Pause->setEnabled(true);
}

void SimpleMediaPlayerWindow::play(void)
{
    if(m_Play)
        return;

    GLMARK_LOG_INFO("Start playing");

    ALL_VIEWTILES setFileSource(m_FileSource);
    ALL_VIEWTILES openFile();
    ALL_LABELS show();
    m_pMsgBar->show();

    m_Play = true;

    on_actionRendering_Video_triggered(ui->actionRendering_Video->isChecked());
    on_actionDecode_Video_triggered(ui->actionDecode_Video->isChecked());
    on_actionDown_Scaling_triggered(ui->actionDown_Scaling->isChecked());
    on_actionTimingControl_triggered(ui->actionTimingControl->isChecked());
    setRate();
    on_action_Hardware_Decode_triggered(ui->action_Hardware_Decode->isChecked());

    m_pTimer->start(1000);

    on_action_Play_triggered();
}

void SimpleMediaPlayerWindow::on_action_Stop_triggered()
{
    ALL_VIEWTILES stopSource();

    m_Stop = true;

    ui->action_Play->setEnabled(true);
    ui->action_Stop->setEnabled(false);
    ui->action_Pause->setEnabled(false);
}

void SimpleMediaPlayerWindow::closeFile(void)
{
    if (!m_Play)
        return;

    GLMARK_LOG_INFO("Stop playing");

    m_pTimer->stop();

    m_Play = false;
    m_Stop = false;

    ALL_VIEWTILES closeFile();
    ALL_LABELS close();
    m_pMsgBar->hide();
}

void SimpleMediaPlayerWindow::on_actionRendering_Video_triggered(bool checked)
{
    if (m_Play)
    {
        ALL_VIEWTILES setRenderingVideo(checked);
        GLMARK_LOG_DEBUG("Set flag rendering video = " << (checked ? "true" : "false"));
    }

    m_RenderingVideo = checked;
}

void SimpleMediaPlayerWindow::disableRenderingVideo(void)
{
    ALL_VIEWTILES setRenderingVideo(false);

    ui->actionRendering_Video->setChecked(false);

    GLMARK_LOG_INFO("Disable rendering video");
}

void SimpleMediaPlayerWindow::on_actionDecode_Video_triggered(bool checked)
{
    if (m_Play)
    {
        ALL_VIEWTILES setDecodeVideo(checked);
        GLMARK_LOG_DEBUG("Set flag decode video = " << (checked ? "true" : "false"));
    }

    m_DecodeVideo = checked;
}

void SimpleMediaPlayerWindow::on_actionDown_Scaling_triggered(bool checked)
{
    if (m_Play)
    {
        ALL_VIEWTILES setDownScaling(checked);
        GLMARK_LOG_DEBUG("Set flag down scaling = " << (checked ? "true" : "false"));
    }

    m_DownScaling = checked;
}

void SimpleMediaPlayerWindow::on_action_Pause_triggered()
{
    ALL_VIEWTILES pauseSource();

    ui->action_Play->setEnabled(true);
    ui->action_Stop->setEnabled(true);
    ui->action_Pause->setEnabled(false);
}

void SimpleMediaPlayerWindow::on_action_Hardware_Decode_triggered(bool checked)
{
    ALL_VIEWTILES setHardwareDecode(checked);

    ALL_VIEWTILES closeFile();

    ALL_VIEWTILES openFile();

    m_HardwareDecode = checked;
}

void SimpleMediaPlayerWindow::enableHardwareDecode(void)
{
    ALL_VIEWTILES setHardwareDecode(true);

    m_HardwareDecode = true;

    ui->action_Hardware_Decode->setChecked(true);

    GLMARK_LOG_INFO("Enable hardware decode");
}

