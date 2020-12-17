#include "RenderingVideoWindow.h"
#include "ui_RenderingVideoWindow.h"
#include <GLVR.h>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <iostream>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <CommonConfig.h>
#include "Screenshot.h"

using surveon::glvr::VideoRendererManager;

RenderingVideoWindow::RenderingVideoWindow(QWidget *parent, size_t row, size_t column,
                                           surveon::mf::MediaSubType pixelFormat, ViewPane::Resolution resolution,
                                           QString exportReportPath, QString saveScreenshotPath,
                                           bool hideMenuBar, bool hideStatusBar,
                                           bool textureUpload, bool showProcessID,
                                           QString description) :
    QMainWindow(parent),
    ui(new Ui::RenderingVideoWindow),
    m_ShowProcessID(showProcessID),
    m_TextureUpload(textureUpload),
    m_Row(row),
    m_Column(column),
    m_VideoBufferSize(1),
    m_TimeCounter(0),
    m_TotalTime(0),
    m_WindowCount(0),
    m_SumCPU(0.0),
    m_Last10sCPU(0.0),
    m_SumFPS(0.0),
    m_Last10sFPS(0.0),
    m_SumBandwidth(0.0),
    m_Last10sBandwidth(0.0),
    m_pTimer(new QTimer(this)),
    m_pAskRow(nullptr),
    m_pAskColumn(nullptr),
    m_Description(description),
    m_ExportReportPath(exportReportPath),
    m_SaveScreenshotPath(saveScreenshotPath),
    m_pMsgBar(new QLabel),
    m_PixelFormat(pixelFormat),
    m_Resolution(resolution)
{
    GLMARK_LOG_INFO("Create RenderingVideoWindow " << SMF_ADDR_PREFIX(this));

    ui->setupUi(this);
    ui->statusBar->addPermanentWidget(m_pMsgBar, true);
    setStyleSheet("QStatusBar::item { border: 0px; }");

    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateStatistics()));

    VideoRendererManager::getInstancePtr()->initialize();
    m_ViewPane.getGLInfo(); // MUST after VideoRendererManager initialization

    createRenderWindows();

    // Test Texture Upload Bandwidth
    if (m_TextureUpload)
    {
        ui->actionFPS->setChecked(false);
        ui->actionBandwidth->setChecked(true);
        ui->actionRGB32->setChecked(true);
    }

    // Set UI of pixel format
    if (m_PixelFormat == surveon::mf::SMF_VIDEO_FORMAT_RGBA)
    {
        ui->actionRGB32->setChecked(true);
    }
    else if (m_PixelFormat == surveon::mf::SMF_VIDEO_FORMAT_YUV420)
    {
        ui->actionYUV420->setChecked(true);
    }
    else if (m_PixelFormat == surveon::mf::SMF_VIDEO_FORMAT_YV12)
    {
        ui->actionYV12->setChecked(true);
    }

    // Set UI of resolution
    if (m_Resolution == ViewPane::RES_CIF)
    {
        ui->actionCIF->setChecked(true);
    }
    else if (m_Resolution == ViewPane::RES_D1)
    {
        ui->actionD1->setChecked(true);
    }
    else if (m_Resolution == ViewPane::RES_720p)
    {
        ui->action720p->setChecked(true);
    }
    else if (m_Resolution == ViewPane::RES_1080p)
    {
        ui->action1080p->setChecked(true);
    }

    // Hide menu bar or not
    if (hideMenuBar)
    {
        ui->menuBar->hide();
    }

    // Hide status bar or not
    if (hideStatusBar)
    {
        ui->statusBar->hide();
    }

    setWindowTitle(description);
}

RenderingVideoWindow::~RenderingVideoWindow()
{
    GLMARK_LOG_INFO("Destroy RenderingVideoWindow");

    VideoRendererManager::getInstancePtr()->shutdown();
    VideoRendererManager::destroyInstance();

    delete m_pTimer;
    delete ui;
}

//===============================================================================================

void RenderingVideoWindow::showEvent(QShowEvent*)
{
    m_pTimer->start(1000);
}

void RenderingVideoWindow::resizeEvent(QResizeEvent *event)
{
    setupLayout();

    reinit();

    QMainWindow::resizeEvent(event);
}

void RenderingVideoWindow::closeEvent(QCloseEvent *event)
{
    m_pTimer->stop();

    for(RenderingWindows::iterator it = m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
    {
        (*it)->close(); // close individual windows
        delete *it;
    }
    m_RenderingWindows.clear(); // clear list

    emit sendClosed();

    GLMARK_LOG_INFO("Close RenderingVideoWindow");

    QMainWindow::closeEvent(event);
}

void RenderingVideoWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        GLMARK_LOG_INFO("Press key 'Esc'");
        setUserTerminate();
    }
}

void RenderingVideoWindow::setUserTerminate()
{
    emit sendUserTerminate();

    this->close();
}

void RenderingVideoWindow::createRenderWindows()
{
    if(m_RenderingWindows.empty())
    {
        size_t count = m_Column * m_Row;
        for(int i=0; i<count; i++ )
        {
            RenderingWindow* pRenderingWindow = new RenderingWindow(ui->centralWidget, m_VideoBufferSize,
                                                                    m_PixelFormat, m_Resolution,
                                                                    m_TextureUpload);
            m_RenderingWindows.push_back(pRenderingWindow);

            connect(ui->actionEnableCanvas, SIGNAL(toggled(bool)), pRenderingWindow, SLOT(setEnableCanvas(bool)));
        }

        m_WindowCount++;
    }
}

void RenderingVideoWindow::setupLayout()
{
    if (m_RenderingWindows.empty())
        return;

    QRect displayArea = ui->centralWidget->geometry();

    int tileLeft = 0;
    int tileTop = 0;
    int tileWidth = (displayArea.width() - tileLeft* 2)/(int)m_Column;
    int tileHeight = (displayArea.height() - tileTop*2)/(int)m_Row;

    QRect tileRect(tileLeft, tileTop, tileWidth, tileHeight);

    RenderingWindows::iterator it = m_RenderingWindows.begin();
    for(int j = 0; j < m_Row; j ++)
    {
        tileRect.moveTop(tileTop + j * tileHeight);

        for(int i = 0; i < m_Column; i ++)
        {
            tileRect.moveLeft(tileLeft + i * tileWidth);
            (*it++)->setGeometry(tileRect);
        }
    }
}

void RenderingVideoWindow::restart()
{
    m_pTimer->stop();

    for(RenderingWindows::iterator it = m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
    {
        (*it)->close(); // close individual windows
        delete *it;
    }
    m_RenderingWindows.clear(); // clear list

    createRenderWindows();

    setupLayout();

    for(RenderingWindows::iterator it = m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
    {
        (*it)->show();
    }

    if (ui->actionEnableCanvas->isChecked())
        ui->actionEnableCanvas->toggled(true);

    m_pTimer->start(1000);

    reinit();
}

void RenderingVideoWindow::reinit()
{
    m_TimeCounter = 0;
    m_SumCPU = 0.0;
    m_Last10sCPU = 0.0;
    m_SumFPS = 0.0;
    m_Last10sFPS = 0.0;
    m_SumBandwidth = 0.0;
    m_Last10sBandwidth = 0.0;
}

//===============================================================================================

void RenderingVideoWindow::on_actionExportReport_triggered()
{
    exportReport(SHOW_MESSAGE_BOX);
}

void RenderingVideoWindow::on_actionScreenshot_triggered()
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

void RenderingVideoWindow::on_actionNewWindow_triggered()
{
    RenderingWindow* pRenderingWindow = new RenderingWindow(0, m_VideoBufferSize, m_PixelFormat, m_Resolution);
    m_RenderingWindows.push_back(pRenderingWindow);

    QDockWidget* dock = new QDockWidget(QString("Window %1").arg(m_WindowCount++), this);

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(pRenderingWindow);
    dock->setMinimumWidth(this->width() / (int)m_WindowCount);
    dock->setMinimumHeight(this->height() / (int)m_WindowCount);

    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void RenderingVideoWindow::on_actionChangeLayout_triggered()
{
    QWidget* popup = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    popup->setAttribute(Qt::WA_DeleteOnClose);

    QLabel *infoRow = new QLabel("Row(1~8):");
    QLineEdit* askRow = new QLineEdit(QString("%1").arg(m_Row));
    QLabel *infoColumn = new QLabel("Column(1~8):");
    QLineEdit* askColumn = new QLineEdit(QString("%1").arg(m_Column));
    QLabel *infoVideoBufferSize = new QLabel("Video Buffer Size(1~30):");
    QLineEdit* askVideoBufferSize = new QLineEdit(QString("%1").arg(m_VideoBufferSize));
    QPushButton *buttonOK = new QPushButton("OK");
    m_pAskRow = askRow;
    m_pAskColumn = askColumn;
    m_pAskVideoBufferSize = askVideoBufferSize;

    connect(buttonOK, SIGNAL(clicked()), popup, SLOT(close()));
    connect(buttonOK, SIGNAL(clicked()), this, SLOT(validateSize()));

    layout->addWidget(infoRow);
    layout->addWidget(askRow);
    layout->addWidget(infoColumn);
    layout->addWidget(askColumn);
    layout->addWidget(infoVideoBufferSize);
    layout->addWidget(askVideoBufferSize);
    layout->addWidget(buttonOK);
    popup->setLayout(layout);
    popup->setWindowTitle("Size");
    popup->show();
}

void RenderingVideoWindow::on_actionCreatePIP_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->createPIP();
    GLMARK_LOG_INFO("Create PIP");
}

void RenderingVideoWindow::on_actionCreateRectangle_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->createRectangle();
    GLMARK_LOG_INFO("Create rectangle");
}

void RenderingVideoWindow::on_actionCreatePolygon_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->createPolygon();
    GLMARK_LOG_INFO("Create polygon");
}

void RenderingVideoWindow::on_actionCreateSprite_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->createSprite();
    GLMARK_LOG_INFO("Create sprite");
}

void RenderingVideoWindow::on_actionDestroyRenderable_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->destroyRenderable();
    GLMARK_LOG_INFO("Destroy renderable");
}

void RenderingVideoWindow::on_actionCreateButton_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->createButton();
    GLMARK_LOG_INFO("Create button");
}

void RenderingVideoWindow::on_actionDestroyButton_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->destroyButton();
    GLMARK_LOG_INFO("Destroy button");
}

void RenderingVideoWindow::on_actionShowButtons_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->showButtons();
    GLMARK_LOG_INFO("Show buttons");
}

void RenderingVideoWindow::on_actionHideButton_triggered()
{
    RenderingWindows::iterator it = m_RenderingWindows.begin();
    (*it)->hideButton();
    GLMARK_LOG_INFO("Hide button");
}

void RenderingVideoWindow::on_actionRGB32_changed()
{
    if (ui->actionRGB32->isChecked())
    {
        // clear others
        clearOthers(surveon::mf::SMF_VIDEO_FORMAT_RGBA);

        // set format
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_PixelFormat = surveon::mf::SMF_VIDEO_FORMAT_RGBA;
            (*it)->setPixelFormat(m_PixelFormat);
        }

        GLMARK_LOG_INFO("Set pixel format to RGB32");

        reinit();
    }
}

void RenderingVideoWindow::on_actionYUV420_changed()
{
    if (ui->actionYUV420->isChecked())
    {
        // clear others
        clearOthers(surveon::mf::SMF_VIDEO_FORMAT_YUV420);

        // set format
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_PixelFormat = surveon::mf::SMF_VIDEO_FORMAT_YUV420;
            (*it)->setPixelFormat(m_PixelFormat);
        }

        GLMARK_LOG_INFO("Set pixel format to YUV420");

        reinit();
    }
}

void RenderingVideoWindow::on_actionYV12_changed()
{
    if (ui->actionYV12->isChecked())
    {
        // clear others
        clearOthers(surveon::mf::SMF_VIDEO_FORMAT_YV12);

        // set format
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_PixelFormat = surveon::mf::SMF_VIDEO_FORMAT_YV12;
            (*it)->setPixelFormat(m_PixelFormat);
        }

        GLMARK_LOG_INFO("Set pixel format to YV12");

        reinit();
    }
}

void RenderingVideoWindow::on_action256x256_changed()
{
    if (ui->action256x256->isChecked())
    {
        // clear others
        clearOthers(ViewPane::RES_256x256);

        // set resolution
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_Resolution = ViewPane::RES_256x256;
            (*it)->setResolution(m_Resolution);
        }

        GLMARK_LOG_INFO("Set resolution to 256 x 256");

        reinit();
    }
}

void RenderingVideoWindow::on_actionCIF_changed()
{
    if (ui->actionCIF->isChecked())
    {
        // clear others
        clearOthers(ViewPane::RES_CIF);

        // set resolution
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_Resolution = ViewPane::RES_CIF;
            (*it)->setResolution(m_Resolution);
        }

        GLMARK_LOG_INFO("Set resolution to CIF");

        reinit();
    }
}

void RenderingVideoWindow::on_action512x512_changed()
{
    if (ui->action512x512->isChecked())
    {
        // clear others
        clearOthers(ViewPane::RES_512x512);

        // set resolution
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_Resolution = ViewPane::RES_512x512;
            (*it)->setResolution(m_Resolution);
        }

        GLMARK_LOG_INFO("Set resolution to 512 x 512");

        reinit();
    }
}

void RenderingVideoWindow::on_actionD1_changed()
{
    if (ui->actionD1->isChecked())
    {
        // clear others
        clearOthers(ViewPane::RES_D1);

        // set resolution
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_Resolution = ViewPane::RES_D1;
            (*it)->setResolution(m_Resolution);
        }

        GLMARK_LOG_INFO("Set resolution to D1");

        reinit();
    }
}

void RenderingVideoWindow::on_action720p_changed()
{
    if (ui->action720p->isChecked())
    {
        // clear others
        clearOthers(ViewPane::RES_720p);

        // set resolution
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_Resolution = ViewPane::RES_720p;
            (*it)->setResolution(m_Resolution);
        }

        GLMARK_LOG_INFO("Set resolution to 720p");

        reinit();
    }
}

void RenderingVideoWindow::on_action1080p_changed()
{
    if (ui->action1080p->isChecked())
    {
        // clear others
        clearOthers(ViewPane::RES_1080p);

        // set resolution
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            m_Resolution = ViewPane::RES_1080p;
            (*it)->setResolution(m_Resolution);
        }

        GLMARK_LOG_INFO("Set resolution to 1080p");

        reinit();
    }
}

void RenderingVideoWindow::on_actionOpenGL_triggered()
{
    GLMARK_LOG_INFO("Show OpenGL info");

    QMessageBox msgBox;
    msgBox.setWindowTitle("OpenGL");
    msgBox.setText("OpenGL Version: " + QString::fromStdString( m_ViewPane.getOpenGLVersion() ) + "\n" +
                   "Shader Version: " + QString::fromStdString( m_ViewPane.getShaderVersion() ));
    msgBox.exec();
}

void RenderingVideoWindow::on_actionSystem_triggered()
{
    GLMARK_LOG_INFO("Show system info");

    QMessageBox msgBox;
    msgBox.setWindowTitle("system");
    msgBox.setText("CPU Model: " + QString::fromStdString( m_ViewPane.getCPUModel() ) + "\n" +
                   "GPU Model: " + QString::fromStdString( m_ViewPane.getGPUModel() ));
    msgBox.exec();
}

void RenderingVideoWindow::updateStatistics()
{
    m_TimeCounter++;

    if (m_RenderingWindows.empty())
        return;

    QString message(" ");

    // Process ID
    if (m_ShowProcessID)
    {
        message += QString("pid: %1   ").arg(QCoreApplication::applicationPid());
    }

    // cpu
    if (ui->actionCPU->isChecked())
    {
        float tmpCPU = m_ViewPane.getProcessCPU();
        message += QString("cpu: %1%/%2%   ").arg(tmpCPU, 0, 'f', 2).arg(m_ViewPane.getTotalCPU(), 0, 'f', 2);
        m_SumCPU += tmpCPU;

        if (tmpCPU > 90.0f)
        {
            GLMARK_LOG_INFO("CPU was consumed too much (more than 90%). Aborted.");
            setUserTerminate();
        }

        if ((m_TimeCounter > m_TotalTime - STATISTICS_TIME) &&
            (m_TimeCounter <= m_TotalTime))
            m_Last10sCPU += tmpCPU;
    }

    // virtual memory
    if (ui->actionMem->isChecked())
    {
        size_t memoryUsed = m_ViewPane.getMemoryUsed();
        message += QString("mem: %1M   ").arg(memoryUsed);
/*
        if (memoryUsed > 2048)
        {
            GLMARK_LOG_INFO("Virtual memory was consumed too much (more than 2G). Aborted.");
            setUserTerminate();
        }
*/
    }

    // fps
    if (ui->actionFPS->isChecked())
    {
        QString fps("fps: ");
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            float tmpFPS = (*it)->getFPS();
            fps += QString("%1   ").arg(tmpFPS, 0, 'f', 2); // float to QString
            m_SumFPS += tmpFPS;

            if ((m_TimeCounter > m_TotalTime - STATISTICS_TIME) &&
                (m_TimeCounter <= m_TotalTime))
                m_Last10sFPS += tmpFPS;
        }
        message += fps;
    }

    // bandwidth
    if (ui->actionBandwidth->isChecked())
    {
        QString bandwidth("bandwidth: ");
        for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
        {
            float tmpBandwidth = (float)(*it)->getBandwidth() / 1073741824; // Bytes to GB
            bandwidth += QString("%1GB/s   ").arg(tmpBandwidth, 0, 'f', 2); // float to QString
            m_SumBandwidth += tmpBandwidth;

            if ((m_TimeCounter > m_TotalTime - STATISTICS_TIME) &&
                (m_TimeCounter <= m_TotalTime))
                m_Last10sBandwidth += tmpBandwidth;
        }
        message += bandwidth;
    }

    m_pMsgBar->setText(message);
}

void RenderingVideoWindow::validateSize()
{
    size_t row = m_pAskRow->text().toULongLong();
    size_t column = m_pAskColumn->text().toULongLong();
    size_t videoBufferSize = m_pAskVideoBufferSize->text().toULongLong();
    if (row>=1 && row<=8 && column>=1 && column<=8 && videoBufferSize>=1 && videoBufferSize<=30) // validate
    {
        if (m_Row!=row || m_Column!=column) // if changed
        {
            m_Row = row;
            m_Column = column;
            GLMARK_LOG_INFO("Change layout to " << m_Row << " x " << m_Column);
            restart();
        }
        else if (m_VideoBufferSize != videoBufferSize)
        {
            m_VideoBufferSize = videoBufferSize;

            // set video buffer size
            for(RenderingWindows::iterator it=m_RenderingWindows.begin(); it!=m_RenderingWindows.end(); ++it)
            {
                (*it)->setVideoBufferSize(m_VideoBufferSize);
            }

            GLMARK_LOG_INFO("Set video buffer size to " << m_VideoBufferSize);
        }
    }
    else
    {
        on_actionChangeLayout_triggered(); // input again
    }
}

void RenderingVideoWindow::exportReport(bool showMessageBox)
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
    size_t tmpTime = showMessageBox ? m_TimeCounter : STATISTICS_TIME;
    float tmpCPU = showMessageBox ? m_SumCPU / m_TimeCounter : m_Last10sCPU / STATISTICS_TIME;
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
                                                                                               \
            <table>                                                                            \
                                                                                               \
                <caption>System Information</caption>                                          \
                                                                                               \
                <tr>                                                                           \
                    <td>Operating System</td> <td>" << m_ViewPane.getOperatingSystem() << "</td> \
                </tr>                                                                          \
                                                                                               \
                <tr class='alt'>                                                               \
                    <td>OpenGL Version</td>   <td>" << m_ViewPane.getOpenGLVersion() << "</td> \
                </tr>                                                                          \
                                                                                               \
                <tr>                                                                           \
                    <td>Shader Version</td>   <td>" << m_ViewPane.getShaderVersion() << "</td> \
                </tr>                                                                          \
                                                                                               \
                <tr class='alt'>                                                               \
                    <td>CPU Model</td>        <td>" << m_ViewPane.getCPUModel() << "</td>      \
                </tr>                                                                          \
                                                                                               \
                <tr>                                                                           \
                    <td>GPU Model</td>        <td>" << m_ViewPane.getGPUModel() << "</td>      \
                </tr>                                                                          \
                                                                                               \
            </table>                                                                           \
                                                                                               \
        </p>                                                                                   \
                                                                                               \
        <p>                                                                                    \
                                                                                               \
            <table>                                                                            \
                                                                                               \
                <caption>Configuration</caption>                                               \
                                                                                               \
                <tr>                                                                           \
                    <td>Layout</td>            <td>" << m_Row << "x" << m_Column << "</td>     \
                </tr>                                                                          \
                                                                                               \
                <tr class='alt'>                                                               \
                    <td>Video Buffer Size</td> <td>" << m_VideoBufferSize << "</td>            \
                </tr>                                                                          \
                                                                                               \
                <tr>                                                                           \
                    <td>Pixel Format</td>      <td>" << g_PixelFormatStrings[m_PixelFormat] << "</td> \
                </tr>                                                                          \
                                                                                               \
                <tr class='alt'>                                                               \
                    <td>Video Resolution</td>  <td>" << g_ResolutionStrings[m_Resolution] << "</td>   \
                </tr>                                                                          \
                                                                                               \
                <tr>                                                                           \
                    <td>Window Size</td>       <td>" << this->width() << "x" << this->height() << "</td>      \
                </tr>                                                                          \
                                                                                               \
            </table>                                                                           \
                                                                                               \
        </p>                                                                                   \
                                                                                               \
        <p>                                                                                    \
                                                                                               \
            <table>                                                                            \
                                                                                               \
                <caption>Statistics</caption>                                                  \
                                                                                               \
                <tr>                                                                           \
                    <td>Time</td>                <td>" << tmpTime << "s</td>                   \
                </tr>                                                                          \
                                                                                               \
                <tr class='alt'>                                                               \
                    <td>Average CPU</td>         <td>" << tmpCPU << "%</td>                    \
                </tr>                                                                          \
                                                                                               \
                <tr>                                                                           \
                    <td>Virtual Memory Used</td> <td>" << m_ViewPane.getMemoryUsed() << "M</td> \
                </tr>                                                                          \
                                                                                               \
    ";

    if (m_TextureUpload)
    {
        float tmp = showMessageBox ? m_SumBandwidth / m_TimeCounter : m_Last10sBandwidth / STATISTICS_TIME;
        ofs << "                                                                               \
                <tr class='alt'>                                                               \
                    <td>Average Texture Upload Bandwidth</td>    <td>" << tmp << "GB/s</td> \
                </tr>                                                                          \
                                                                                               \
            </table>                                                                           \
                                                                                               \
        </p>                                                                                   \
                                                                                               \
    </body>                                                                                    \
                                                                                               \
</html>	                                                                                       \
        ";
    }
    else
    {
        float tmpTFPS = showMessageBox ? m_SumFPS / m_TimeCounter : m_Last10sFPS / STATISTICS_TIME;
        float tmpFPS = showMessageBox ? m_SumFPS / (m_TimeCounter * m_Row * m_Column) : m_Last10sFPS / (STATISTICS_TIME * m_Row * m_Column);
        ofs << "                                                                               \
                <tr class='alt'>                                                               \
                    <td>Average Total FPS</td>            <td>" << tmpTFPS << "</td>           \
                </tr>                                                                          \
                                                                                               \
                <tr>                                                                           \
                    <td>Average FPS</td>                  <td>" << tmpFPS << "</td>            \
                </tr>                                                                          \
                                                                                               \
            </table>                                                                           \
                                                                                               \
        </p>                                                                                   \
                                                                                               \
    </body>                                                                                    \
                                                                                               \
</html>	                                                                                       \
        ";
    }

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

void RenderingVideoWindow::activeRenderingVideoWindow()
{
    QApplication::setActiveWindow(this);
}

void RenderingVideoWindow::quickshot()
{
    Screenshot screenshot(m_ExportReportPath, m_SaveScreenshotPath);
    screenshot.quickshot(m_Description);

    GLMARK_LOG_INFO("Save screenshot in " << m_SaveScreenshotPath);
}

void RenderingVideoWindow::clearOthers(ViewPane::Resolution resolution)
{
    if (resolution != ViewPane::RES_256x256)
        ui->action256x256->setChecked(false);

    if (resolution != ViewPane::RES_CIF)
        ui->actionCIF->setChecked(false);

    if (resolution != ViewPane::RES_512x512)
        ui->action512x512->setChecked(false);

    if (resolution != ViewPane::RES_D1)
        ui->actionD1->setChecked(false);

    if (resolution != ViewPane::RES_720p)
        ui->action720p->setChecked(false);

    if (resolution != ViewPane::RES_1080p)
        ui->action1080p->setChecked(false);
}

void RenderingVideoWindow::clearOthers(surveon::mf::MediaSubType format)
{
    if (format != surveon::mf::SMF_VIDEO_FORMAT_RGBA)
        ui->actionRGB32->setChecked(false);

    if (format != surveon::mf::SMF_VIDEO_FORMAT_YUV420)
        ui->actionYUV420->setChecked(false);

    if (format != surveon::mf::SMF_VIDEO_FORMAT_YV12)
        ui->actionYV12->setChecked(false);
}




