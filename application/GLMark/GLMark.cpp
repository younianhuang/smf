
#include "GLMark.h"
#include "ui_GLMark.h"

#include <QEventLoop>

using surveon::glvr::VideoRendererManager;

GLMark::GLMark(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GLMark),
    m_bUserTerminate(false),
    m_bRunAll(false),
    m_ExportReportPath(QDir::currentPath()),
    m_SaveScreenshotPath(QDir::currentPath())
{
    GLMARK_LOG_INFO("Create GLmark " << SMF_ADDR_PREFIX(this));

    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 30px; }");
    ui->lineEditExportReport->setText(m_ExportReportPath);
    ui->lineEditSaveScreenshot->setText(m_SaveScreenshotPath);

    connect(this, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
    connect(ui->comboBoxResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenFrequencyUI(int)));
    connect(ui->comboBoxTestResolution, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenFrequencyUI(int)));

    VideoRendererManager::getInstancePtr()->initialize();

    m_XMLManager.readXMLFile();

    int layoutRow = m_XMLManager.getLayoutRow().toInt();
    int layoutColumn = m_XMLManager.getLayoutColumn().toInt();
    if ((layoutRow >= 1) && (layoutRow <= 8) &&
        (layoutColumn >= 1) && (layoutColumn <= 8))
    {
        ui->row_spin->setValue(layoutRow);
        ui->column_spin->setValue(layoutColumn);
    }

    if (m_XMLManager.getPixelFormat() == "RGB32")
    {
        ui->radio_rgb32->setChecked(true);
        ui->radio_yuv420->setChecked(false);
        ui->radio_yv12->setChecked(false);
    }
    else if (m_XMLManager.getPixelFormat() == "YUV420")
    {
        ui->radio_rgb32->setChecked(false);
        ui->radio_yuv420->setChecked(true);
        ui->radio_yv12->setChecked(false);
    }
    else if (m_XMLManager.getPixelFormat() == "YV12")
    {
        ui->radio_rgb32->setChecked(false);
        ui->radio_yuv420->setChecked(false);
        ui->radio_yv12->setChecked(true);
    }

    if (m_XMLManager.getVideoResolution() == "CIF")
    {
        ui->radio_CIF->setChecked(true);
        ui->radio_D1->setChecked(false);
        ui->radio_720p->setChecked(false);
        ui->radio_1080p->setChecked(false);
    }
    else if (m_XMLManager.getVideoResolution() == "D1")
    {
        ui->radio_CIF->setChecked(false);
        ui->radio_D1->setChecked(true);
        ui->radio_720p->setChecked(false);
        ui->radio_1080p->setChecked(false);
    }
    else if (m_XMLManager.getVideoResolution() == "720p")
    {
        ui->radio_CIF->setChecked(false);
        ui->radio_D1->setChecked(false);
        ui->radio_720p->setChecked(true);
        ui->radio_1080p->setChecked(false);
    }
    else if (m_XMLManager.getVideoResolution() == "1080p")
    {
        ui->radio_CIF->setChecked(false);
        ui->radio_D1->setChecked(false);
        ui->radio_720p->setChecked(false);
        ui->radio_1080p->setChecked(true);
    }

    if (!m_XMLManager.getExportReportPath().isEmpty())
        ui->lineEditExportReport->setText(m_XMLManager.getExportReportPath());

    if (!m_XMLManager.getSaveScreenshotPath().isEmpty())
        ui->lineEditSaveScreenshot->setText(m_XMLManager.getSaveScreenshotPath());

    setupScreenConfigUI(ui->comboBoxResolution);
    setupScreenConfigUI(ui->comboBoxTestResolution);

    ui->group_process->setVisible(false);
    connect(ui->radio_mode_multiple, SIGNAL(toggled(bool)), ui->group_process, SLOT(setVisible(bool)));
    connect(ui->radio_mode_video, SIGNAL(toggled(bool)), ui->group_screen_test, SLOT(setVisible(bool)));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(showCurrentItem(int)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openEditor(QListWidgetItem*)));

    getTestConf();

    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(deleteCurrentItem()));
    connect(ui->pushButtonEdit, SIGNAL(clicked()), this, SLOT(editCurrentItem()));
    connect(ui->pushButtonUp, SIGNAL(clicked()), this, SLOT(moveUpCurrentItem()));
    connect(ui->pushButtonDown, SIGNAL(clicked()), this, SLOT(moveDownCurrentItem()));

    ui->spin_thread_number->setMaximum( m_CPUUsage.getNumProcessors() ); // set maximum thread number

    ui->tabWidget->insertTab(4, ui->tab_test_configuration, "Test Configuration"); // reorder tab widgets

    // Load XML Conf: Decode Sample & Set UI
    // Layout
    int decodeLayoutRow = m_XMLManager.getDecodeLayoutRow().toInt();
    int decodeLayoutColumn = m_XMLManager.getDecodeLayoutColumn().toInt();
    if ((decodeLayoutRow >= 1) && (decodeLayoutRow <= 8) &&
        (decodeLayoutColumn >= 1) && (decodeLayoutColumn <= 8))
    {
        ui->row_spin_decode->setValue(decodeLayoutRow);
        ui->column_spin_decode->setValue(decodeLayoutColumn);
    }
    // Flag
    ui->checkBoxRenderingVideo->setChecked(m_XMLManager.getRenderingVideo() == "Enable");
    ui->checkBoxDecodeVideo->setChecked(m_XMLManager.getDecodeVideo() == "Enable");
    ui->checkBoxDownScaling->setChecked(m_XMLManager.getDownScaling() == "Enable");
    // Thread Number
    ui->spin_thread_number->setValue(m_XMLManager.getThreadNumber().toInt());
    // Timing Control
    ui->checkBoxTimingControlEnable->setChecked(m_XMLManager.getTimingControl() == "Enable");
    ui->doubleSpinBox->setValue(m_XMLManager.getRate().toDouble());
}

GLMark::~GLMark()
{
    VideoRendererManager::getInstancePtr()->shutdown();
    VideoRendererManager::destroyInstance();

    delete ui;

    GLMARK_LOG_INFO("Destroy GLmark");
}

int GLMark::exeRunAll()
{
    GLMARK_LOG_INFO("Execute 'GLMark -a'");

    on_pushButtonRunAll_clicked();

    keepChangingLayout();

    demoRenderable();

    GLMARK_LOG_INFO("Finish 'GLMark -a'");

    if (m_bUserTerminate)
    {
        GLMARK_LOG_INFO("m_bUserTerminate = true. Aborted. Return 1.");
        return 1;
    }
    else
        return 0;
}

void GLMark::keyPressEvent(QKeyEvent* pEvent)
{
    if (pEvent->key() == Qt::Key_Escape)
    {
        GLMARK_LOG_INFO("Press key Escape");

        emit sendUserTerminate();

        this->close();
    }
    else if ((pEvent->key() == Qt::Key_Return) || (pEvent->key() == Qt::Key_Enter))
    {
        ui->listWidget->closePersistentEditor(ui->listWidget->currentItem());
        setCurrentItemStr();
    }
    else if (pEvent->key() == Qt::Key_F2)
    {
        GLMARK_LOG_INFO("Press key F2");

        editCurrentItem();
    }
    else if (pEvent->key() == Qt::Key_Delete)
    {
        GLMARK_LOG_INFO("Press key Delete");

        deleteCurrentItem();
    }
}

void GLMark::editCurrentItem()
{
    QListWidgetItem* pCurrentItem = ui->listWidget->currentItem();
    ui->listWidget->openPersistentEditor(pCurrentItem);
}

void GLMark::deleteCurrentItem()
{
    if (ui->listWidget->count() <= 1)
        return;

    int currentRow = ui->listWidget->currentRow();
    QListWidgetItem *pItem = ui->listWidget->takeItem(currentRow);
    GLMARK_LOG_INFO("Delete test conf '" << pItem->text() << "'");

    for (TestConfs::iterator it=m_TestConfs.begin(); it!=m_TestConfs.end(); it++)
    {
        if (pItem == (*it)->pItem)
        {
            delete pItem;
            pItem = nullptr;
            m_TestConfs.remove(*it);
            break;
        }
    }
}

void GLMark::moveUpCurrentItem()
{
    // check boundary condition
    int currentRow = ui->listWidget->currentRow();
    if (currentRow <= 0)
        return;

    // move widget
    QListWidgetItem *pItem = ui->listWidget->takeItem(currentRow);
    GLMARK_LOG_INFO("Move up test conf '" << pItem->text() << "' to row " << currentRow - 1);
    ui->listWidget->insertItem(currentRow - 1, pItem);

    // move struct
    for (TestConfs::iterator it=m_TestConfs.begin(); it!=m_TestConfs.end(); it++)
    {
        if (pItem == (*it)->pItem)
        {
            TestConfs::iterator itCurrent = it;
            TestConf *pTestConfcurrent = *it--;

            // Swap
            *itCurrent = *it;
            *it = pTestConfcurrent;

            break;
        }
    }

    ui->listWidget->setCurrentRow(currentRow - 1);
}

void GLMark::moveDownCurrentItem()
{
    // check boundary condition
    int currentRow = ui->listWidget->currentRow();
    if (currentRow >= (int)m_TestConfs.size() - 1)
        return;

    // move widget
    QListWidgetItem* pItem = ui->listWidget->takeItem(currentRow);
    GLMARK_LOG_INFO("Move down test conf '" << pItem->text() << "' to row " << currentRow + 1);
    ui->listWidget->insertItem(currentRow + 1, pItem);

    // move struct
    for (TestConfs::iterator it=m_TestConfs.begin(); it!=m_TestConfs.end(); it++)
    {
        if (pItem == (*it)->pItem)
        {
            TestConfs::iterator itCurrent = it;
            TestConf *pTestConfcurrent = *it++;

            // Swap
            *itCurrent = *it;
            *it = pTestConfcurrent;

            break;
        }
    }

    ui->listWidget->setCurrentRow(currentRow + 1);
}

void GLMark::closeEvent(QCloseEvent* pEvent)
{
    GLMARK_LOG_INFO("Close GLmark");

    clearRenderingVideoWindows();
    clearTestConfs();
    clearSimpleMediaPlayerWindows();

    QMainWindow::closeEvent(pEvent);
}

void GLMark::on_pushButtonGeneralTest_clicked()
{
    GLMARK_LOG_INFO("Run Rendering Sample");

    size_t row = ui->row_spin->text().toInt();
    size_t column = ui->column_spin->text().toInt();

    // Get Pixel Format
    surveon::mf::MediaSubType pixelFormat = surveon::mf::SMF_VIDEO_FORMAT_YV12;
    if (ui->radio_rgb32->isChecked())
    {
        pixelFormat = surveon::mf::SMF_VIDEO_FORMAT_RGBA;
    }
    else if (ui->radio_yuv420->isChecked())
    {
        pixelFormat = surveon::mf::SMF_VIDEO_FORMAT_YUV420;
    }
    else if (ui->radio_yv12->isChecked())
    {
        pixelFormat = surveon::mf::SMF_VIDEO_FORMAT_YV12;
    }

    // Get Video Resolution
    ViewPane::Resolution resolution = ViewPane::RES_1080p;
    if (ui->radio_CIF->isChecked())
    {
        resolution = ViewPane::RES_CIF;
    }
    else if (ui->radio_D1->isChecked())
    {
        resolution = ViewPane::RES_D1;
    }
    else if (ui->radio_720p->isChecked())
    {
        resolution = ViewPane::RES_720p;
    }
    else if (ui->radio_1080p->isChecked())
    {
        resolution = ViewPane::RES_1080p;
    }

    // Get Selected Resolution Index
    unsigned int selectedResolutionIndex = ui->comboBoxResolution->currentIndex();

    // Get Selected Frequency Index
    unsigned int selectedFrequencyIndex = ui->comboBoxFrequency->currentIndex();

    // Show Confirmation
    int selectedFrequency = m_ScreenSetter.getFrequency(selectedResolutionIndex, selectedFrequencyIndex);
    int currentResolutionIndex = m_ScreenSetter.getCurrentSizeID();
    int currentFrequency = m_ScreenSetter.getCurrentRate();

    if ((currentResolutionIndex == (int)selectedResolutionIndex) && (currentFrequency == selectedFrequency))
    {
        QString description("Rendering Sample");
        RenderingVideoWindow* pRenderingVideoWindow =
                new RenderingVideoWindow(NO_PARENT, row, column, pixelFormat, resolution,
                                         m_ExportReportPath, m_SaveScreenshotPath,
                                         !HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                         !SHOW_PROCESS_ID, description);
        pRenderingVideoWindow->setGeometry(300, 300, this->width(), this->height());
        pRenderingVideoWindow->setWindowTitle(description);

        pRenderingVideoWindow->showMaximized();

        m_RenderingVideoWindows.push_back(pRenderingVideoWindow);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Set Screen %1 x %2 @ %3 Hz")
                       .arg(m_ScreenSetter.getScreenSizeWidth(selectedResolutionIndex))
                       .arg(m_ScreenSetter.getScreenSizeHeight(selectedResolutionIndex))
                       .arg(selectedFrequency));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        // Set Screen Resolution
        if (ret == QMessageBox::Ok)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            m_ScreenSetter.setScreenResolution(selectedResolutionIndex, selectedFrequencyIndex);

            std::this_thread::sleep_for(std::chrono::seconds(2));

            QString description(QString("General Test %1 x %2 @ %3 Hz")
                                .arg(m_ScreenSetter.getScreenSizeWidth(selectedResolutionIndex))
                                .arg(m_ScreenSetter.getScreenSizeHeight(selectedResolutionIndex))
                                .arg(selectedFrequency));
            RenderingVideoWindow* pRenderingVideoWindow =
                    new RenderingVideoWindow(NO_PARENT, row, column, pixelFormat, resolution,
                                             m_ExportReportPath, m_SaveScreenshotPath,
                                             !HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                             !SHOW_PROCESS_ID, description);
            pRenderingVideoWindow->setGeometry(300, 300, this->width(), this->height());
            pRenderingVideoWindow->showMaximized();

            m_RenderingVideoWindows.push_back(pRenderingVideoWindow);
        }
    }

    GLMARK_LOG_INFO("Finish Rendering Sample");
}

void GLMark::on_pushButtonMultipleWindows_clicked()
{
    GLMARK_LOG_INFO("Run Multiple Windows");

    for (int row=2; row<=8; row+=2)
    {
        if (((row == 2) && (!ui->checkBox2x2->isChecked())) ||
            ((row == 4) && (!ui->checkBox4x4->isChecked())) ||
            ((row == 6) && (!ui->checkBox6x6->isChecked())) ||
            ((row == 8) && (!ui->checkBox8x8->isChecked())))
        {
            continue;
        }

        if (!m_bUserTerminate)
        {
            QString description(QString("Multiple Windows %1 x %2").arg(row).arg(row));
            RenderingVideoWindow* pRenderingVideoWindow =
                    new RenderingVideoWindow(NO_PARENT, row, row, surveon::mf::SMF_VIDEO_FORMAT_YV12, ViewPane::RES_1080p,
                                             m_ExportReportPath, m_SaveScreenshotPath,
                                             HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                             !SHOW_PROCESS_ID, description);
            pRenderingVideoWindow->setCursor(Qt::BlankCursor);
            description = "GLMark - " + description + " (Press ESC to quit)";
            pRenderingVideoWindow->setWindowTitle(description);
            pRenderingVideoWindow->showMaximized();

            m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

            QEventLoop eventLoop;

            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

            pRenderingVideoWindow->setTotalTime(TOTAL_TIME);
            QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
            QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
            QTimer::singleShot(CLOSE_TIME, pRenderingVideoWindow, SLOT(close()));
            connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

            eventLoop.exec();
        }
    }

    if (!m_bRunAll)
        m_bUserTerminate = false;

    GLMARK_LOG_INFO("Finish Multiple Windows");
}

void GLMark::on_pushButtonMultipleWindows_db_clicked()
{
    foreach (int row, QList<int>() << 1 << 2 << 4 << 6 << 8)
    {
        if (((row == 1) && (!ui->checkBox1x1_db->isChecked())) ||
            ((row == 2) && (!ui->checkBox2x2_db->isChecked())) ||
            ((row == 4) && (!ui->checkBox4x4_db->isChecked())) ||
            ((row == 6) && (!ui->checkBox6x6_db->isChecked())) ||
            ((row == 8) && (!ui->checkBox8x8_db->isChecked())))
        {
            continue;
        }

        if (!m_bUserTerminate)
        {
            GLMARK_LOG_INFO("Run Decode Benchmark - Multiple Windows " << row << " x " << row);

            // Configuration
            const bool flagRenderingVideo = false;
            const bool flagDecodeVideo = true;
            const bool flagDownScaling = true;
            const int numberOfThread = 4;
            const bool flagTimingControlEnable = false;
            const float rate = 1.3f;

            // Create
            QString description(QString("Decode Multiple Windows %1 x %2").arg(row).arg(row));
            SimpleMediaPlayerWindow *pSimpleMediaPlayerWindow =
                    new SimpleMediaPlayerWindow(NO_PARENT, row, row,
                                                flagRenderingVideo, flagDecodeVideo, flagDownScaling,
                                                numberOfThread, flagTimingControlEnable, rate,
                                                m_ExportReportPath, m_SaveScreenshotPath,
                                                description);
            pSimpleMediaPlayerWindow->setCursor(Qt::BlankCursor);
            description = "GLMark - " + description + " (Press ESC to quit)";
            pSimpleMediaPlayerWindow->setWindowTitle(description);

            // set file source & play
            pSimpleMediaPlayerWindow->enableHardwareDecode();
            pSimpleMediaPlayerWindow->setFileSource("..//resource//test1080p.avi");
            pSimpleMediaPlayerWindow->play();

            pSimpleMediaPlayerWindow->showMaximized();

            // Collect
            m_SimpleMediaPlayerWindows.push_back(pSimpleMediaPlayerWindow);

            // Loop
            QEventLoop eventLoop;

            connect(pSimpleMediaPlayerWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
            connect(pSimpleMediaPlayerWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

            pSimpleMediaPlayerWindow->setTotalTime(TOTAL_TIME);
            connect(pSimpleMediaPlayerWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

            eventLoop.exec();
            GLMARK_LOG_INFO("Finish Decode Benchmark - Multiple Windows " << row << " x " << row);
        }
    }

    if (!m_bRunAll)
        m_bUserTerminate = false;
}

void GLMark::on_pushButtonRunDecodeSample_clicked()
{
    // Configuration
    size_t row = ui->row_spin_decode->value();
    size_t column = ui->column_spin_decode->value();
    bool flagRenderingVideo = ui->checkBoxRenderingVideo->isChecked();
    bool flagDecodeVideo = ui->checkBoxDecodeVideo->isChecked();
    bool flagDownScaling = ui->checkBoxDownScaling->isChecked();
    int numberOfThread = ui->spin_thread_number->value();
    bool flagTimingControlEnable = ui->checkBoxTimingControlEnable->isChecked();
    float rate = ui->doubleSpinBox->value();

    // Create
    QString description("Decode Sample");
    SimpleMediaPlayerWindow *pSimpleMediaPlayerWindow =
            new SimpleMediaPlayerWindow(NO_PARENT, row, column,
                                        flagRenderingVideo, flagDecodeVideo, flagDownScaling,
                                        numberOfThread, flagTimingControlEnable, rate,
                                        m_ExportReportPath, m_SaveScreenshotPath,
                                        description);
    pSimpleMediaPlayerWindow->setWindowTitle(description);

    pSimpleMediaPlayerWindow->showMaximized();
    GLMARK_LOG_INFO("Run Decode Sample");

    // Collect
    m_SimpleMediaPlayerWindows.push_back(pSimpleMediaPlayerWindow);
}

void GLMark::keepChangingLayout()
{
    GLMARK_LOG_INFO("Run Stress Test: Keep Changing Layout");

    for (int times=1; times<=10; times++)
    {
        for (int row=1; row<=4; row++)
        {
            for (int column=1; column<=4; column++)
            {
                if (!m_bUserTerminate)
                {
                    QString description("Keep Changing Layout");
                    RenderingVideoWindow* pRenderingVideoWindow =
                            new RenderingVideoWindow(NO_PARENT, row, column,
                                                     surveon::mf::SMF_VIDEO_FORMAT_YV12, ViewPane::RES_1080p,
                                                     m_ExportReportPath, m_SaveScreenshotPath,
                                                     HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                                     !SHOW_PROCESS_ID, description);
                    pRenderingVideoWindow->setCursor(Qt::BlankCursor);
                    description = "GLMark - " + description + " (Press ESC to quit)";
                    pRenderingVideoWindow->setWindowTitle(description);
                    pRenderingVideoWindow->showMaximized();

                    m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

                    QEventLoop eventLoop;

                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

//                    pRenderingVideoWindow->setTotalTime(3);
//                    QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
//                    QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
                    QTimer::singleShot(3100, pRenderingVideoWindow, SLOT(close()));
                    connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

                    eventLoop.exec();
                }
            }
        }
    }

    GLMARK_LOG_INFO("Finish Stress Test: Keep Changing Layout");
}

void GLMark::demoRenderable()
{
    GLMARK_LOG_INFO("Run Stress Test: Demo Renderable");

    for (int times=1; times<=10; times++)
    {
        for (int row=1; row<=2; row++)
        {
            for (int column=1; column<=2; column++)
            {
                if (!m_bUserTerminate)
                {
                    QString description("Demo Renderable");
                    RenderingVideoWindow* pRenderingVideoWindow =
                            new RenderingVideoWindow(NO_PARENT, row, column,
                                                     surveon::mf::SMF_VIDEO_FORMAT_YV12, ViewPane::RES_1080p,
                                                     m_ExportReportPath, m_SaveScreenshotPath,
                                                     HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                                     !SHOW_PROCESS_ID, description);
                    pRenderingVideoWindow->setCursor(Qt::BlankCursor);
                    description = "GLMark - " + description + " (Press ESC to quit)";
                    pRenderingVideoWindow->setWindowTitle(description);
                    pRenderingVideoWindow->showMaximized();

                    m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

                    QEventLoop eventLoop;

                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

                    // Start
                    if (qrand() % 2)
                        QTimer::singleShot(1000, pRenderingVideoWindow, SLOT(on_actionCreatePIP_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(2000, pRenderingVideoWindow, SLOT(on_actionCreateRectangle_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(3000, pRenderingVideoWindow, SLOT(on_actionCreatePolygon_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(4000, pRenderingVideoWindow, SLOT(on_actionCreateSprite_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(5000, pRenderingVideoWindow, SLOT(on_actionDestroyRenderable_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(6000, pRenderingVideoWindow, SLOT(on_actionCreateButton_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(7000, pRenderingVideoWindow, SLOT(on_actionDestroyButton_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(8000, pRenderingVideoWindow, SLOT(on_actionShowButtons_triggered()));
                    if (qrand() % 2)
                        QTimer::singleShot(9000, pRenderingVideoWindow, SLOT(on_actionHideButton_triggered()));

//                    pRenderingVideoWindow->setTotalTime(3);
//                    QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
//                    QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
                    QTimer::singleShot(10000, pRenderingVideoWindow, SLOT(close()));
                    connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

                    eventLoop.exec();
                }
            }
        }
    }

    GLMARK_LOG_INFO("Finish Stress Test: Demo Renderable");
}

void GLMark::on_pushButtonTextureUpload_clicked()
{
    GLMARK_LOG_INFO("Run Texture Upload");

    for (int i=0; i<ViewPane::RES_COUNT; i++)
    {
        if (((i == ViewPane::RES_256x256) && (!ui->checkBox256x256->isChecked())) ||
            ((i == ViewPane::RES_CIF)     && (!ui->checkBoxCIF->isChecked())) ||
            ((i == ViewPane::RES_512x512) && (!ui->checkBox512x512->isChecked())) ||
            ((i == ViewPane::RES_D1)      && (!ui->checkBoxD1->isChecked())) ||
            ((i == ViewPane::RES_720p)    && (!ui->checkBox720p->isChecked())) ||
            ((i == ViewPane::RES_1080p)   && (!ui->checkBox1080p->isChecked())))
        {
            continue;
        }

        if (!m_bUserTerminate)
        {
            QString description(QString("Texture Upload %1").arg(
                                    QString::fromStdString(g_ResolutionStrings[i])));
            RenderingVideoWindow* pRenderingVideoWindow =
                    new RenderingVideoWindow(NO_PARENT, 1, 1,
                                             surveon::mf::SMF_VIDEO_FORMAT_YV12, (ViewPane::Resolution)i,
                                             m_ExportReportPath, m_SaveScreenshotPath,
                                             HIDE_MENU_BAR, !HIDE_STATUS_BAR, TEXTURE_UPLOAD,
                                             !SHOW_PROCESS_ID, description);
            pRenderingVideoWindow->setCursor(Qt::BlankCursor);
            description = "GLMark - " + description + " (Press ESC to quit)";
            pRenderingVideoWindow->setWindowTitle(description);
            pRenderingVideoWindow->showMaximized();

            m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

            QEventLoop eventLoop;

            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

            pRenderingVideoWindow->setTotalTime(TOTAL_TIME);
            QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
            QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
            QTimer::singleShot(CLOSE_TIME, pRenderingVideoWindow, SLOT(close()));
            connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

            eventLoop.exec();
        }
    }

    if (!m_bRunAll)
        m_bUserTerminate = false;

    GLMARK_LOG_INFO("Finish Texture Upload");
}

void GLMark::on_pushButtonDifferentResolution_clicked()
{
    GLMARK_LOG_INFO("Run Different Resolution");

    for (int i=0; i<ViewPane::RES_COUNT; i++)
    {
        if (((i == ViewPane::RES_256x256) && (!ui->checkBox256x256DifferentResolution->isChecked())) ||
            ((i == ViewPane::RES_CIF)     && (!ui->checkBoxCIFDifferentResolution->isChecked())) ||
            ((i == ViewPane::RES_512x512) && (!ui->checkBox512x512DifferentResolution->isChecked())) ||
            ((i == ViewPane::RES_D1)      && (!ui->checkBoxD1DifferentResolution->isChecked())) ||
            ((i == ViewPane::RES_720p)    && (!ui->checkBox720pDifferentResolution->isChecked())) ||
            ((i == ViewPane::RES_1080p)   && (!ui->checkBox1080pDifferentResolution->isChecked())))
        {
            continue;
        }

        if (!m_bUserTerminate)
        {
            QString description(QString("Different Resolution %1").arg(
                                    QString::fromStdString(g_ResolutionStrings[i])));
            RenderingVideoWindow* pRenderingVideoWindow =
                    new RenderingVideoWindow(NO_PARENT, 4, 4,
                                             surveon::mf::SMF_VIDEO_FORMAT_YV12, (ViewPane::Resolution)i,
                                             m_ExportReportPath, m_SaveScreenshotPath,
                                             HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                             !SHOW_PROCESS_ID, description);
            pRenderingVideoWindow->setCursor(Qt::BlankCursor);
            description = "GLMark - " + description + " (Press ESC to quit)";
            pRenderingVideoWindow->setWindowTitle(description);
            pRenderingVideoWindow->showMaximized();

            m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

            QEventLoop eventLoop;

            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

            pRenderingVideoWindow->setTotalTime(TOTAL_TIME);
            QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
            QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
            QTimer::singleShot(CLOSE_TIME, pRenderingVideoWindow, SLOT(close()));
            connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

            eventLoop.exec();
        }
    }

    if (!m_bRunAll)
        m_bUserTerminate = false;

    GLMARK_LOG_INFO("Finish Different Resolution");
}

void GLMark::on_pushButtonDifferentFormat_clicked()
{
    GLMARK_LOG_INFO("Run Different Format");

    for (int i=0; i<surveon::mf::SMF_SUB_TYPE_NONE; i++)
    {
        if (((i == surveon::mf::SMF_VIDEO_FORMAT_RGBA)  && (!ui->checkBoxRGB32->isChecked())) ||
            ((i == surveon::mf::SMF_VIDEO_FORMAT_YUV420) && (!ui->checkBoxYUV420->isChecked())) ||
            ((i == surveon::mf::SMF_VIDEO_FORMAT_YV12)   && (!ui->checkBoxYV12->isChecked())))
        {
            continue;
        }

        if (!m_bUserTerminate)
        {
            QString description(QString("Different Format %1").arg(
                                    QString::fromStdString(g_PixelFormatStrings[i])));
            RenderingVideoWindow* pRenderingVideoWindow =
                    new RenderingVideoWindow(NO_PARENT, 4, 4,
                                             (surveon::mf::MediaSubType)i, ViewPane::RES_1080p,
                                             m_ExportReportPath, m_SaveScreenshotPath,
                                             HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                             !SHOW_PROCESS_ID, description);
            pRenderingVideoWindow->setCursor(Qt::BlankCursor);
            description = "GLMark - " + description + " (Press ESC to quit)";
            pRenderingVideoWindow->setWindowTitle(description);
            pRenderingVideoWindow->showMaximized();

            m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

            QEventLoop eventLoop;

            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

            pRenderingVideoWindow->setTotalTime(TOTAL_TIME);
            QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
            QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
            QTimer::singleShot(CLOSE_TIME, pRenderingVideoWindow, SLOT(close()));
            connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

            eventLoop.exec();
        }
    }

    if (!m_bRunAll)
        m_bUserTerminate = false;

    GLMARK_LOG_INFO("Finish Different Format");
}

void GLMark::on_pushButtonMultipleMonitors_clicked()
{
    runMultipleMonitors(SHOW_MESSAGE_BOX);

    if (!m_bRunAll)
        m_bUserTerminate = false;
}

void GLMark::on_pushButtonRunAll_clicked()
{
    GLMARK_LOG_INFO("Run Rendering Benchmark");

    m_bRunAll = true;

    on_pushButtonMultipleWindows_clicked();

    on_pushButtonTextureUpload_clicked();

    on_pushButtonDifferentResolution_clicked();

    on_pushButtonDifferentFormat_clicked();

    runMultipleMonitors();

    m_bRunAll = false;
    m_bUserTerminate = false;

    GLMARK_LOG_INFO("Finish Rendering Benchmark");
}

void GLMark::on_pushButtonExportReport_clicked()
{
    QString path = ui->lineEditExportReport->text();
    if (path.isEmpty())
        path = QDir::currentPath();

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    path,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        ui->lineEditExportReport->setText(dir);
        m_ExportReportPath = dir;
    }

    GLMARK_LOG_INFO("Change Export Report Path");
}

void GLMark::on_pushButtonSaveScreenshot_clicked()
{
    QString path = ui->lineEditSaveScreenshot->text();
    if (path.isEmpty())
        path = QDir::currentPath();

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    path,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        ui->lineEditSaveScreenshot->setText(dir);
        m_SaveScreenshotPath = dir;
    }

    GLMARK_LOG_INFO("Change Save Screenshot Path");
}

void GLMark::updateScreenFrequencyUI(int index)
{
    const int numRates = m_ScreenSetter.getNumRates(index);
    const int currentSizeID = m_ScreenSetter.getCurrentSizeID();
    const int currentRateID = m_ScreenSetter.getCurrentRateID();

    QObject* obj = this->sender();
    QComboBox* pComboboxResolution = dynamic_cast<QComboBox*>(obj);
    if (!pComboboxResolution)
        return;

    QComboBox *pComboboxFrequency = nullptr;
    if (pComboboxResolution == ui->comboBoxResolution)
        pComboboxFrequency = ui->comboBoxFrequency;
    else if (pComboboxResolution == ui->comboBoxTestResolution)
        pComboboxFrequency = ui->comboBoxTestFrequency;

    pComboboxFrequency->clear(); // clear combobox of frequency

    int userFrequencyIndex = -1;
    for (int i=0; i<numRates; i++)
    {
        int frequency = m_ScreenSetter.getFrequency(index, i);

        pComboboxFrequency->addItem(QString("%1 Hz").arg(frequency));

        if (pComboboxResolution == ui->comboBoxResolution &&
            frequency == m_XMLManager.getScreenFrequency().toInt())
            userFrequencyIndex = i;
    }

    if (userFrequencyIndex != -1)
    {
        pComboboxFrequency->setCurrentIndex(userFrequencyIndex);
    }
    else if (index == currentSizeID)
    {
        pComboboxFrequency->setCurrentIndex(currentRateID);
    }
    else
    {
        pComboboxFrequency->setCurrentIndex(0); // set index 0 as default
    }
}

void GLMark::setUserTerminate()
{
    m_bUserTerminate = true;

    GLMARK_LOG_INFO("User Terminate");
}

void GLMark::clearRenderingVideoWindows()
{
    // Clear RenderingVideoWindows
    for (RenderingVideoWindows::iterator it=m_RenderingVideoWindows.begin(); it!=m_RenderingVideoWindows.end(); ++it)
    {
        (*it)->close();
    }
    m_RenderingVideoWindows.clear();
}

void GLMark::clearTestConfs()
{
    for (TestConfs::iterator it=m_TestConfs.begin(); it!=m_TestConfs.end(); it++)
    {
        delete *it;
    }
    m_TestConfs.clear();
}

void GLMark::clearSimpleMediaPlayerWindows()
{
    for (SimpleMediaPlayerWindows::iterator it=m_SimpleMediaPlayerWindows.begin();
                                            it!=m_SimpleMediaPlayerWindows.end(); ++it)
    {
        if (!(*it)->isHidden())
            (*it)->close();
//        delete *it;
//        *it = nullptr;
    }
    m_SimpleMediaPlayerWindows.clear();
}

void GLMark::setupScreenConfigUI(QComboBox* pCombobox)
{
    // Resolution
    const int count = m_ScreenSetter.getNumSizes();
    int currentSizeID = m_ScreenSetter.getCurrentSizeID();

    for (int i=0; i<count; i++)
    {
        pCombobox->addItem(QString("%1 x %2").arg(m_ScreenSetter.getScreenSizeWidth(i))
                                             .arg(m_ScreenSetter.getScreenSizeHeight(i)));

        if (pCombobox == ui->comboBoxResolution &&
            m_ScreenSetter.getScreenSizeWidth(i)  == m_XMLManager.getScreenResolutionWidth().toInt() &&
            m_ScreenSetter.getScreenSizeHeight(i) == m_XMLManager.getScreenResolutionHeight().toInt())
            currentSizeID = i;
    }

    // Show Current Resolution
    pCombobox->setCurrentIndex(currentSizeID); // trigger updateScreenFrequencyUI()
}

void GLMark::runMultipleMonitors(bool showMessageBox)
{
    // Check Requirements
    bool virtualDesktop = QApplication::desktop()->isVirtualDesktop();
    int screenCount = QApplication::desktop()->screenCount();

    if (m_bUserTerminate)
        return;

    if ((!virtualDesktop) || (screenCount < 2))
    {
        if (showMessageBox)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Multiple Monitors Setup Error");
            msgBox.setText("Please connect to another monitor.");
            msgBox.exec();
        }

        return;
    }

    if (m_bUserTerminate)
        return;

    GLMARK_LOG_INFO("Run Multiple Monitors");

    // Single Process
    if (ui->checkBoxSingleProcess->isChecked())
    {
        QEventLoop eventLoop;

        int left = 0;
        for (int i=0; i<screenCount; i++)
        {
            const QRect screenGeometry = QApplication::desktop()->screenGeometry(i);

            QString description(QString("Single Process Screen %1").arg(i));
            RenderingVideoWindow* pRenderingVideoWindow =
                    new RenderingVideoWindow(NO_PARENT, 4, 4,
                                             surveon::mf::SMF_VIDEO_FORMAT_YV12, ViewPane::RES_1080p,
                                             m_ExportReportPath, m_SaveScreenshotPath,
                                             HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                             SHOW_PROCESS_ID, description);
            pRenderingVideoWindow->setCursor(Qt::BlankCursor);
            description = "GLMark - " + description + " (Press ESC to quit)";
            pRenderingVideoWindow->setWindowTitle(description);
            pRenderingVideoWindow->setGeometry(left, 0, screenGeometry.width(), screenGeometry.height());
            pRenderingVideoWindow->showMaximized();

            m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(clearRenderingVideoWindows()));
            connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

            pRenderingVideoWindow->setTotalTime(TOTAL_TIME);
            QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
            if (i == 0)
                QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
            QTimer::singleShot(CLOSE_TIME, pRenderingVideoWindow, SLOT(close()));
            connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

            left +=  screenGeometry.width();
        }

        eventLoop.exec();
    }

    if (m_bUserTerminate)
        return;

    // Multiple Processes
    if (ui->checkBoxMultipleProcesses->isChecked())
    {
        QEventLoop eventLoop;

        // Show RenderingVideoWindow on Monitors in Separate Processes
        for (int i=1; i<screenCount; i++) // Begin with Screen Number 1
        {
            QProcess* pMyProcess = new QProcess;
            pMyProcess->start("./GLMark", QStringList() << "-s" << QString("%1").arg(i)); // Pass Screen Number
        }

        // Primary Screen in Current Process
        QString description(QString("Multiple Processes Screen 0"));
        RenderingVideoWindow* pRenderingVideoWindow =
                new RenderingVideoWindow(NO_PARENT, 4, 4,
                                         surveon::mf::SMF_VIDEO_FORMAT_YV12, ViewPane::RES_1080p,
                                         m_ExportReportPath, m_SaveScreenshotPath,
                                         HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                         SHOW_PROCESS_ID, description);
        pRenderingVideoWindow->setCursor(Qt::BlankCursor);
        description = "GLMark - " + description + " (Press ESC to quit)";
        pRenderingVideoWindow->setWindowTitle(description);
        pRenderingVideoWindow->showMaximized();

        m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

        connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
        connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(clearRenderingVideoWindows()));
//        connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), pMyProcess, SLOT(kill()));
        connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

        pRenderingVideoWindow->setTotalTime(TOTAL_TIME);
        QTimer::singleShot(2000, pRenderingVideoWindow, SLOT(activeRenderingVideoWindow())); // Set to Top Level
        QTimer::singleShot(EXPORT_TIME, pRenderingVideoWindow, SLOT(exportReport()));
        QTimer::singleShot(SHOOT_TIME, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
        QTimer::singleShot(CLOSE_TIME, pRenderingVideoWindow, SLOT(close()));

        eventLoop.exec();
    }

    GLMARK_LOG_INFO("Finish Multiple Monitors");
}

void GLMark::on_pushButtonSaveRenderingSample_clicked()
{
    saveConfXML("LAYOUT", QString("%1x%2").arg(ui->row_spin->value()) // int to QString
                                          .arg(ui->column_spin->value()));

    saveConfXML("PIXEL_FORMAT", ui->radio_rgb32->isChecked()  ? "RGB32"  :
                                ui->radio_yuv420->isChecked() ? "YUV420" :
                                ui->radio_yv12->isChecked()   ? "YV12"   : "YV12");

    saveConfXML("VIDEO_RESOLUTION", ui->radio_CIF->isChecked()   ? "CIF"   :
                                    ui->radio_D1->isChecked()    ? "D1"    :
                                    ui->radio_720p->isChecked()  ? "720p"  :
                                    ui->radio_1080p->isChecked() ? "1080p" : "1080p");

    saveConfXML("SCREEN_RESOLUTION", ui->comboBoxResolution->currentText().replace(" ", ""));

    saveConfXML("SCREEN_FREQUENCY", ui->comboBoxFrequency->currentText().replace(" ", ""));

    showMessageBoxXMLSaved();

    GLMARK_LOG_INFO("Save Rendering Sample Configuration");
}

void GLMark::showMessageBoxXMLSaved()
{
    QMessageBox* pMsgBox = new QMessageBox;
    pMsgBox->setAttribute(Qt::WA_DeleteOnClose);
    pMsgBox->setWindowTitle("Configuration Saved Successfully");
    pMsgBox->setText("Save as " + QDir::currentPath() + "/GLMark.xml");
    pMsgBox->setButtonText(QMessageBox::Ok, "&Close");
    pMsgBox->show();

    QTimer::singleShot(3000, pMsgBox, SLOT(close()));
}

void GLMark::on_pushButtonSaveGeneralOptions_clicked()
{
    saveConfXML("EXPORT_REPORT_PATH", ui->lineEditExportReport->text());

    saveConfXML("SAVE_SCREENSHOT_PATH", ui->lineEditSaveScreenshot->text());

    showMessageBoxXMLSaved();

    GLMARK_LOG_INFO("Save General Options Configuration");
}

void GLMark::saveConfXML(QString tagName, QString text)
{
    // read from file
    QDomDocument doc("GLMark");
    QFile inFile("GLMark.xml");
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if (!doc.setContent(&inFile))
    {
        inFile.close();
        return;
    }
    inFile.close();

    // modify XML
    QDomNode node = doc.documentElement().elementsByTagName(tagName).at(0);
    QDomNode textNode = node.toElement().firstChild();
    QDomText newTextNode = doc.createTextNode(text);
    node.appendChild(newTextNode);
    node.removeChild(textNode);

    // write back to file
    QFile outFile("GLMark.xml");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream( &outFile );
    stream << doc.toString();
    outFile.close();
}

void GLMark::on_pushButtonAdd_clicked()
{
    const int currentIndex = ui->tabWidgetType->currentIndex();
    const QString type = ui->tabWidgetType->tabText(currentIndex);
    GLMARK_LOG_DEBUG("type = " << type);

    QString str;
    QXmlStreamWriter xmlWriter(&str);
    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartElement("TEST");

    xmlWriter.writeTextElement("NAME", QString("TestConf%1").arg(m_TestConfs.size()));

    if (type == "Rendering")
    {
        xmlWriter.writeTextElement("TYPE", "Rendering");

        if (ui->radio_mode_video->isChecked())
        {
            xmlWriter.writeTextElement("MODE", "video");

            xmlWriter.writeTextElement("LAYOUT", QString("%1x%2").arg(ui->row_spin_test->value())
                                                                 .arg(ui->column_spin_test->value()));

            xmlWriter.writeTextElement("PIXEL_FORMAT", getPixelFormatStr());

            xmlWriter.writeTextElement("VIDEO_RESOLUTION", getVideoResolutionStr());

            xmlWriter.writeTextElement("SCREEN_RESOLUTION", ui->comboBoxTestResolution->currentText().replace(" ", ""));

            xmlWriter.writeTextElement("SCREEN_FREQUENCY", ui->comboBoxTestFrequency->currentText().replace(" ", ""));
        }
        else if (ui->radio_mode_texture->isChecked())
        {
            xmlWriter.writeTextElement("MODE", "texture");

            xmlWriter.writeTextElement("VIDEO_RESOLUTION", getVideoResolutionStr());
        }
        else if (ui->radio_mode_multiple->isChecked())
        {
            xmlWriter.writeTextElement("MODE", "multiple");

            xmlWriter.writeTextElement("LAYOUT", QString("%1x%2").arg(ui->row_spin_test->value())
                                                                 .arg(ui->column_spin_test->value()));

            xmlWriter.writeTextElement("PIXEL_FORMAT", getPixelFormatStr());

            xmlWriter.writeTextElement("VIDEO_RESOLUTION", getVideoResolutionStr());

            QString tmp = ui->radio_process_single->isChecked() ? "SingleProcessForAllMonitors" :
                          ui->radio_process_one->isChecked()    ? "OneProcessForEachMonitor" : "OneProcessForEachMonitor";
            xmlWriter.writeTextElement("PROCESS_SETTING", tmp);
        }

        xmlWriter.writeTextElement("TIME", QString("%1s").arg(ui->time_spin_test->value()));
    }
    else if (type == "Decode")
    {
        xmlWriter.writeTextElement("TYPE", "Decode");

        xmlWriter.writeTextElement("DECODE_LAYOUT", QString("%1x%2").arg(ui->row_spin_decode_test->value())
                                                                    .arg(ui->column_spin_decode_test->value()));

        xmlWriter.writeTextElement("RENDERING_VIDEO", ui->checkBoxRenderingVideo_test->isChecked() ? "Enable" : "Disable");
        xmlWriter.writeTextElement("DECODE_VIDEO",    ui->checkBoxDecodeVideo_test->isChecked() ? "Enable" : "Disable");
        xmlWriter.writeTextElement("DOWN_SCALING",    ui->checkBoxDownScaling_test->isChecked() ? "Enable" : "Disable");
        xmlWriter.writeTextElement("THREAD_NUMBER",   QString("%1").arg(ui->spin_thread_number_test->value())); // int to QString
        xmlWriter.writeTextElement("TIMING_CONTROL",  ui->checkBoxTimingControlEnable_test->isChecked() ? "Enable" : "Disable");
        xmlWriter.writeTextElement("RATE",            QString("%1").arg(ui->doubleSpinBox_test->value())); // double to QString

        xmlWriter.writeTextElement("DECODE_TIME", QString("%1s").arg(ui->time_spin_test_decode->value()));
    }

    xmlWriter.writeEndElement(); // Close TEST

    QListWidgetItem* pListWidgetItem = new QListWidgetItem;
    QString name(QString("TestConf%1").arg(m_TestConfs.size()));
    pListWidgetItem->setText(name);
    GLMARK_LOG_INFO("Add test conf '" << name << "'");
    ui->listWidget->addItem(pListWidgetItem);
    ui->listWidget->openPersistentEditor(pListWidgetItem);

    TestConf* pTestConf = new TestConf;
    pTestConf->pItem = pListWidgetItem;
    pTestConf->str = str;
    GLMARK_LOG_DEBUG(name << "->str = " << str);
    m_TestConfs.push_back(pTestConf);

    ui->listWidget->setCurrentRow((int)m_TestConfs.size() - 1);
}

void GLMark::setTestVideoResolution()
{
    if (m_XMLVideoResolution == "256x256")
    {
        ui->radio_256x256_test->setChecked(true);
    }
    else if (m_XMLVideoResolution == "CIF")
    {
        ui->radio_CIF_test->setChecked(true);
    }
    else if (m_XMLVideoResolution == "512x512")
    {
        ui->radio_512x512_test->setChecked(true);
    }
    else if (m_XMLVideoResolution == "D1")
    {
        ui->radio_D1_test->setChecked(true);
    }
    else if (m_XMLVideoResolution == "720p")
    {
        ui->radio_720p_test->setChecked(true);
    }
    else if (m_XMLVideoResolution == "1080p")
    {
        ui->radio_1080p_test->setChecked(true);
    }
}

void GLMark::setTestPixelFormat()
{
    if (m_XMLPixelFormat == "RGB32")
    {
        ui->radio_rgb32_test->setChecked(true);
    }
    else if (m_XMLPixelFormat == "YUV420")
    {
        ui->radio_yuv420_test->setChecked(true);
    }
    else if (m_XMLPixelFormat == "YV12")
    {
        ui->radio_yv12_test->setChecked(true);
    }
}

void GLMark::showCurrentItem(int index)
{
    TestConfs::iterator it=m_TestConfs.begin();
    std::advance(it, index);

    parseXMLStr((*it)->str);

    // set UI
    if (m_XMLType == "Rendering")
    {
        ui->tabWidgetType->setCurrentIndex(0);

        if (m_XMLMode == "video")
        {
            // set mode
            ui->radio_mode_video->setChecked(true);

            // set layout
            ui->row_spin_test->setValue(m_XMLLayoutRow.toInt());
            ui->column_spin_test->setValue(m_XMLLayoutColumn.toInt());

            // set pixel format
            setTestPixelFormat();

            // set video resolution
            setTestVideoResolution();

            // set screen resolution
            int index = 0;
            const int count = m_ScreenSetter.getNumSizes();
            for (int i=0; i<count; i++)
            {
                if (m_ScreenSetter.getScreenSizeWidth(i)  == m_XMLScreenSizeWidth.toInt() &&
                    m_ScreenSetter.getScreenSizeHeight(i) == m_XMLScreenSizeHeight.toInt())
                {
                    ui->comboBoxTestResolution->setCurrentIndex(i);
                    index = i;
                    break;
                }
            }

            // set screen frequency
            const int numRates = m_ScreenSetter.getNumRates(index);
            for (int i=0; i<numRates; i++)
            {
                if (m_XMLFrequency.toInt() == m_ScreenSetter.getFrequency(index, i))
                {
                    ui->comboBoxTestFrequency->setCurrentIndex(i);
                    break;
                }
            }
        }
        else if (m_XMLMode == "texture")
        {
            ui->radio_mode_texture->setChecked(true);

            // set video resolution
            setTestVideoResolution();
        }
        else if (m_XMLMode == "multiple")
        {
            ui->radio_mode_multiple->setChecked(true);

            // set layout
            ui->row_spin_test->setValue(m_XMLLayoutRow.toInt());
            ui->column_spin_test->setValue(m_XMLLayoutColumn.toInt());

            // set pixel format
            setTestPixelFormat();

            // set video resolution
            setTestVideoResolution();

            // set process setting
            if (m_XMLProcessSetting == "SingleProcessForAllMonitors")
            {
                ui->radio_process_single->setChecked(true);
            }
            else if (m_XMLProcessSetting == "OneProcessForEachMonitor")
            {
                ui->radio_process_one->setChecked(true);
            }
        }

        // set time
        ui->time_spin_test->setValue(m_XMLTime.toInt());
    }
    else if (m_XMLType == "Decode")
    {
        ui->tabWidgetType->setCurrentIndex(1);

        // set layout
        ui->row_spin_decode_test->setValue(m_XMLDecodeLayoutRow.toInt());
        ui->column_spin_decode_test->setValue(m_XMLDecodeLayoutColumn.toInt());

        // set flags
        ui->checkBoxRenderingVideo_test->setChecked(m_XMLRenderingVideo == "Enable" ? true : false);
        ui->checkBoxDecodeVideo_test->setChecked(m_XMLDecodeVideo == "Enable" ? true : false);
        ui->checkBoxDownScaling_test->setChecked(m_XMLDownScaling == "Enable" ? true : false);

        // set thread number
        ui->spin_thread_number_test->setValue(m_XMLThreadNumber.toDouble());

        // set timing control
        ui->checkBoxTimingControlEnable_test->setChecked(m_XMLTimingControl == "Enable" ? true : false);
        ui->doubleSpinBox_test->setValue(m_XMLRate.toDouble());

        // set time
        ui->time_spin_test_decode->setValue(m_XMLDecodeTime.toInt());
    }
}

inline QString GLMark::getXMLtext(QDomDocument *pDoc, QString tagName)
{
    return pDoc->documentElement().elementsByTagName(tagName).at(0).toElement().text();
}

void GLMark::parseXMLStr(const QString& str)
{
    // read XML string
    QDomDocument doc;
    if (!doc.setContent(str)) // read XML from QString
        return;

    // parse XML
    m_XMLName = getXMLtext(&doc, "NAME");
    m_XMLType = getXMLtext(&doc, "TYPE");
    if (m_XMLType == "Rendering")
    {
        m_XMLMode = getXMLtext(&doc, "MODE");
        if (m_XMLMode == "video")
        {
            // set layout
            QStringList layout = getXMLtext(&doc, "LAYOUT").split("x");
            m_XMLLayoutRow = layout.at(0);
            m_XMLLayoutColumn = layout.at(1);

            // set pixel format
            m_XMLPixelFormat = getXMLtext(&doc, "PIXEL_FORMAT");

            // set video resolution
            m_XMLVideoResolution = getXMLtext(&doc, "VIDEO_RESOLUTION");

            // set screen resolution
            QStringList resolution = getXMLtext(&doc, "SCREEN_RESOLUTION").split("x");
            m_XMLScreenSizeWidth = resolution.at(0);
            m_XMLScreenSizeHeight = resolution.at(1);

            // set screen frequency
            m_XMLFrequency =  getXMLtext(&doc, "SCREEN_FREQUENCY").split("Hz").at(0);
        }
        else if (m_XMLMode == "texture")
        {
            // set video resolution
            m_XMLVideoResolution = getXMLtext(&doc, "VIDEO_RESOLUTION");
        }
        else if (m_XMLMode == "multiple")
        {
            // set layout
            QStringList layout = getXMLtext(&doc, "LAYOUT").split("x");
            m_XMLLayoutRow = layout.at(0);
            m_XMLLayoutColumn = layout.at(1);

            // set pixel format
            m_XMLPixelFormat = getXMLtext(&doc, "PIXEL_FORMAT");

            // set video resolution
            m_XMLVideoResolution = getXMLtext(&doc, "VIDEO_RESOLUTION");

            // set process setting
            m_XMLProcessSetting = getXMLtext(&doc, "PROCESS_SETTING");
        }

        // set time
        m_XMLTime = getXMLtext(&doc, "TIME").split("s").at(0);
    }
    else if (m_XMLType == "Decode")
    {
        // set layout
        QStringList decodeLayout = getXMLtext(&doc, "DECODE_LAYOUT").split("x");
        m_XMLDecodeLayoutRow = decodeLayout.at(0);
        m_XMLDecodeLayoutColumn = decodeLayout.at(1);

        // set flags
        m_XMLRenderingVideo = getXMLtext(&doc, "RENDERING_VIDEO");
        m_XMLDecodeVideo = getXMLtext(&doc, "DECODE_VIDEO");
        m_XMLDownScaling = getXMLtext(&doc, "DOWN_SCALING");

        // set thread number
        m_XMLThreadNumber = getXMLtext(&doc, "THREAD_NUMBER");

        // set timing control
        m_XMLTimingControl = getXMLtext(&doc, "TIMING_CONTROL");
        m_XMLRate = getXMLtext(&doc, "RATE");

        // set time
        m_XMLDecodeTime = getXMLtext(&doc, "DECODE_TIME").split("s").at(0);
    }
}

void GLMark::on_radio_mode_texture_toggled(bool checked)
{
    if (checked)
    {
        ui->radio_yv12_test->setChecked(true);
    }

    ui->group_layout->setVisible(!checked);
    ui->group_pix_test->setEnabled(!checked);
}

QString GLMark::getVideoResolutionStr()
{
    return ui->radio_256x256_test->isChecked() ? "256x256" :
           ui->radio_CIF_test->isChecked()     ? "CIF"     :
           ui->radio_512x512_test->isChecked() ? "512x512" :
           ui->radio_D1_test->isChecked()      ? "D1"      :
           ui->radio_720p_test->isChecked()    ? "720p"    :
           ui->radio_1080p_test->isChecked()   ? "1080p"   : "1080p";
}

QString GLMark::getPixelFormatStr()
{
    return ui->radio_rgb32_test->isChecked()  ? "RGB32"  :
           ui->radio_yuv420_test->isChecked() ? "YUV420" :
           ui->radio_yv12_test->isChecked()   ? "YV12"   : "YV12";
}

void GLMark::editConfXML(QDomDocument* pDoc, QString tagName, QString text)
{
    QDomNode node = pDoc->documentElement().elementsByTagName(tagName).at(0);
    QDomNode textNode = node.toElement().firstChild();
    QDomText newTextNode = pDoc->createTextNode(text);

    // log name of test conf
    if ((tagName == "NAME") && (node.toElement().text() != text)) // if rename
        GLMARK_LOG_INFO("Rename test conf '" << node.toElement().text() <<
                        "' as '" << text << "'");

    node.appendChild(newTextNode);
    node.removeChild(textNode);
}

void GLMark::setCurrentItemStr()
{
    QListWidgetItem* pItem = ui->listWidget->currentItem();
    for (TestConfs::iterator it=m_TestConfs.begin(); it!=m_TestConfs.end(); it++)
    {
        if (pItem == (*it)->pItem)
        {
            // read TestConf
            QDomDocument doc;
            if (!doc.setContent((*it)->str)) // read XML from QString
                return;

            // modify conf
            editConfXML(&doc, "NAME", pItem->text());

            const int currentIndex = ui->tabWidgetType->currentIndex();
            const QString type = ui->tabWidgetType->tabText(currentIndex);
            if (type == "Rendering")
            {
                if (ui->radio_mode_video->isChecked() && (*it)->str.contains("<MODE>video</MODE>"))
                {
                    editConfXML(&doc, "LAYOUT", QString("%1x%2").arg(ui->row_spin_test->value())
                                                                .arg(ui->column_spin_test->value()));
                    editConfXML(&doc, "PIXEL_FORMAT", getPixelFormatStr());
                    editConfXML(&doc, "VIDEO_RESOLUTION", getVideoResolutionStr());
                    editConfXML(&doc, "SCREEN_RESOLUTION", ui->comboBoxTestResolution->currentText().replace(" ", ""));
                    editConfXML(&doc, "SCREEN_FREQUENCY", ui->comboBoxTestFrequency->currentText().replace(" ", ""));
                }
                else if (ui->radio_mode_texture->isChecked() && (*it)->str.contains("<MODE>texture</MODE>"))
                {
                    editConfXML(&doc, "VIDEO_RESOLUTION", getVideoResolutionStr());
                }
                else if (ui->radio_mode_multiple->isChecked() && (*it)->str.contains("<MODE>multiple</MODE>"))
                {
                    editConfXML(&doc, "LAYOUT", QString("%1x%2").arg(ui->row_spin_test->value())
                                                                .arg(ui->column_spin_test->value()));
                    editConfXML(&doc, "PIXEL_FORMAT", getPixelFormatStr());
                    editConfXML(&doc, "VIDEO_RESOLUTION", getVideoResolutionStr());

                    QString tmp = ui->radio_process_single->isChecked() ? "SingleProcessForAllMonitors" :
                                  ui->radio_process_one->isChecked()    ? "OneProcessForEachMonitor" : "OneProcessForEachMonitor";
                    editConfXML(&doc, "PROCESS_SETTING", tmp);
                }

                editConfXML(&doc, "TIME", QString("%1").arg(ui->time_spin_test->value()));
            }
            else if (type == "Decode")
            {
                editConfXML(&doc, "DECODE_LAYOUT", QString("%1x%2").arg(ui->row_spin_decode_test->value())
                                                                   .arg(ui->column_spin_decode_test->value()));

                editConfXML(&doc, "RENDERING_VIDEO", ui->checkBoxRenderingVideo_test->isChecked() ? "Enable" : "Disable");
                editConfXML(&doc, "DECODE_VIDEO",    ui->checkBoxDecodeVideo_test->isChecked() ? "Enable" : "Disable");
                editConfXML(&doc, "DOWN_SCALING",    ui->checkBoxDownScaling_test->isChecked() ? "Enable" : "Disable");
                editConfXML(&doc, "THREAD_NUMBER",   QString("%1").arg(ui->spin_thread_number_test->value())); // int to QString
                editConfXML(&doc, "TIMING_CONTROL",  ui->checkBoxTimingControlEnable_test->isChecked() ? "Enable" : "Disable");
                editConfXML(&doc, "RATE",            QString("%1").arg(ui->doubleSpinBox_test->value())); // double to QString

                editConfXML(&doc, "DECODE_TIME", QString("%1").arg(ui->time_spin_test_decode->value()));
            }
            else // change mode
            {
                QString name = pItem->text();
                GLMARK_LOG_INFO("Change mode for test conf '" << name << "'");

                surveon::glmark::Log::setLogOn(false); // turn off log

                int currentRow = ui->listWidget->currentRow();
                on_pushButtonAdd_clicked();
                ui->listWidget->currentItem()->setText(name);

#if defined(KeyPress) && defined(SMF_PLATFORM_LINUX)
    #undef KeyPress
#endif

                QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);

#if !defined(KeyPress) && defined(SMF_PLATFORM_LINUX)
    #define KeyPress 2 // X11/X.h
#endif

                QApplication::sendEvent(this, &keyEvent);
                ui->listWidget->setCurrentRow(currentRow);
                deleteCurrentItem();
                ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
                for (int i=0; i<(ui->listWidget->count() - 1 - currentRow); i++)
                    moveUpCurrentItem();

                surveon::glmark::Log::setLogOn(true); // turn on log

                return;
            }

            GLMARK_LOG_INFO("Set test conf '" << pItem->text() << "'");
            GLMARK_LOG_DEBUG(pItem->text() << "->str = " << (*it)->str);

            // write back to string
            (*it)->str = doc.toString();

            break;
        }
    }
}

void GLMark::openEditor(QListWidgetItem *pItem)
{
    ui->listWidget->openPersistentEditor(pItem);
}

void GLMark::on_pushButtonRunTestConf_clicked()
{
    for (TestConfs::iterator it=m_TestConfs.begin(); it!=m_TestConfs.end(); it++)
    {
        parseXMLStr((*it)->str);

        GLMARK_LOG_INFO("Run test conf '" << m_XMLName << "'");

        QString description((*it)->pItem->text());
        surveon::mf::MediaSubType videoFormat = m_XMLPixelFormat == "RGB32"  ? surveon::mf::SMF_VIDEO_FORMAT_RGBA   :
                                                m_XMLPixelFormat == "YUV420" ? surveon::mf::SMF_VIDEO_FORMAT_YUV420 :
                                                m_XMLPixelFormat == "YV12"   ? surveon::mf::SMF_VIDEO_FORMAT_YV12   : surveon::mf::SMF_VIDEO_FORMAT_YV12;
        ViewPane::Resolution resolution = m_XMLVideoResolution == "256x256" ? ViewPane::RES_256x256 :
                                          m_XMLVideoResolution == "CIF"     ? ViewPane::RES_CIF     :
                                          m_XMLVideoResolution == "512x512" ? ViewPane::RES_512x512 :
                                          m_XMLVideoResolution == "D1"      ? ViewPane::RES_D1      :
                                          m_XMLVideoResolution == "720p"    ? ViewPane::RES_720p    :
                                          m_XMLVideoResolution == "1080p"   ? ViewPane::RES_1080p   : ViewPane::RES_1080p;

        if (m_XMLType == "Rendering")
        {
            if (m_XMLMode == "video")
            {
               RenderingVideoWindow* pRenderingVideoWindow =
                       new RenderingVideoWindow(NO_PARENT, m_XMLLayoutRow.toInt(), m_XMLLayoutColumn.toInt(),
                                                videoFormat, resolution,
                                                m_ExportReportPath, m_SaveScreenshotPath,
                                                HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                                !SHOW_PROCESS_ID, description);

                pRenderingVideoWindow->setCursor(Qt::BlankCursor);
                description = "GLMark - " + description + " (Press ESC to quit)";
                pRenderingVideoWindow->setWindowTitle(description);
                pRenderingVideoWindow->showMaximized();

                m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

                QEventLoop eventLoop;

                connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(clearRenderingVideoWindows()));
                connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

                pRenderingVideoWindow->setTotalTime(m_XMLTime.toInt());
                QTimer::singleShot(m_XMLTime.toInt()*1000-2000, pRenderingVideoWindow, SLOT(exportReport()));
                QTimer::singleShot(m_XMLTime.toInt()*1000-1000, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
                QTimer::singleShot(m_XMLTime.toInt()*1000, pRenderingVideoWindow, SLOT(close()));
                connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

                eventLoop.exec();
            }
            else if (m_XMLMode == "texture")
            {
                RenderingVideoWindow* pRenderingVideoWindow =
                        new RenderingVideoWindow(NO_PARENT, 1, 1,
                                                 surveon::mf::SMF_VIDEO_FORMAT_YV12, resolution,
                                                 m_ExportReportPath, m_SaveScreenshotPath,
                                                 HIDE_MENU_BAR, !HIDE_STATUS_BAR, TEXTURE_UPLOAD,
                                                 !SHOW_PROCESS_ID, description);

                pRenderingVideoWindow->setCursor(Qt::BlankCursor);
                description = "GLMark - " + description + " (Press ESC to quit)";
                pRenderingVideoWindow->setWindowTitle(description);
                pRenderingVideoWindow->showMaximized();

                m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

                QEventLoop eventLoop;

                connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(clearRenderingVideoWindows()));
                connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

                pRenderingVideoWindow->setTotalTime(m_XMLTime.toInt());
                QTimer::singleShot(m_XMLTime.toInt()*1000-2000, pRenderingVideoWindow, SLOT(exportReport()));
                QTimer::singleShot(m_XMLTime.toInt()*1000-1000, pRenderingVideoWindow, SLOT(quickshot())); // Capture Snapshot
                QTimer::singleShot(m_XMLTime.toInt()*1000, pRenderingVideoWindow, SLOT(close()));
                connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

                eventLoop.exec();
            }
            else if (m_XMLMode == "multiple")
            {
                if (m_XMLProcessSetting == "SingleProcessForAllMonitors")
                {
                    QEventLoop eventLoop;

                    int left = 0;
                    const int screenCount = QApplication::desktop()->screenCount();
                    for (int i=0; i<screenCount; i++)
                    {
                        const QRect screenGeometry = QApplication::desktop()->screenGeometry(i);

                        RenderingVideoWindow* pRenderingVideoWindow =
                                new RenderingVideoWindow(NO_PARENT, m_XMLLayoutRow.toInt(), m_XMLLayoutColumn.toInt(),
                                                         videoFormat, resolution,
                                                         m_ExportReportPath, m_SaveScreenshotPath,
                                                         HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                                         SHOW_PROCESS_ID, description);
                        pRenderingVideoWindow->setCursor(Qt::BlankCursor);
                        description = "GLMark - " + description + " (Press ESC to quit)";
                        pRenderingVideoWindow->setWindowTitle(description);
                        pRenderingVideoWindow->setGeometry(left, 0, screenGeometry.width(), screenGeometry.height());
                        pRenderingVideoWindow->showMaximized();

                        m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

                        connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
                        connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(clearRenderingVideoWindows()));
                        connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

                        pRenderingVideoWindow->setTotalTime(m_XMLTime.toInt());
                        QTimer::singleShot(m_XMLTime.toInt()*1000-2000, pRenderingVideoWindow, SLOT(exportReport()));
                        if (i == 0)
                            QTimer::singleShot(m_XMLTime.toInt()*1000-1000, pRenderingVideoWindow, SLOT(quickshot())); // Snapshot
                        QTimer::singleShot(m_XMLTime.toInt()*1000, pRenderingVideoWindow, SLOT(close()));
                        connect(pRenderingVideoWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

                        left +=  screenGeometry.width();
                    }

                    eventLoop.exec();
                }
                else if (m_XMLProcessSetting == "OneProcessForEachMonitor")
                {
                    QEventLoop eventLoop;

                    // Show RenderingVideoWindow on Monitors in Separate Processes
                    const int screenCount = QApplication::desktop()->screenCount();
                    for (int i=1; i<screenCount; i++) // Begin with Screen Number 1
                    {
                        QProcess* pMyProcess = new QProcess;
                        pMyProcess->start("./GLMark", QStringList() << "-s" << QString("%1").arg(i)); // Pass Screen Number
                    }

                    // Primary Screen in Current Process
                    RenderingVideoWindow* pRenderingVideoWindow =
                            new RenderingVideoWindow(NO_PARENT, m_XMLLayoutRow.toInt(), m_XMLLayoutColumn.toInt(),
                                                     videoFormat, resolution,
                                                     m_ExportReportPath, m_SaveScreenshotPath,
                                                     HIDE_MENU_BAR, !HIDE_STATUS_BAR, !TEXTURE_UPLOAD,
                                                     SHOW_PROCESS_ID, description);
                    pRenderingVideoWindow->setCursor(Qt::BlankCursor);
                    description = "GLMark - " + description + " (Press ESC to quit)";
                    pRenderingVideoWindow->setWindowTitle(description);
                    pRenderingVideoWindow->showMaximized();

                    m_RenderingVideoWindows.push_back(pRenderingVideoWindow);

                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(setUserTerminate()));
                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), this, SLOT(clearRenderingVideoWindows()));
//                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), pMyProcess, SLOT(kill()));
                    connect(pRenderingVideoWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

                    pRenderingVideoWindow->setTotalTime(m_XMLTime.toInt());
                    QTimer::singleShot(2000, pRenderingVideoWindow, SLOT(activeRenderingVideoWindow())); // Set to Top Level
                    QTimer::singleShot(m_XMLTime.toInt()*1000-2000, pRenderingVideoWindow, SLOT(exportReport()));
                    QTimer::singleShot(m_XMLTime.toInt()*1000-1000, pRenderingVideoWindow, SLOT(quickshot())); // Snapshot
                    QTimer::singleShot(m_XMLTime.toInt()*1000, pRenderingVideoWindow, SLOT(close()));

                    eventLoop.exec();
                }
            }
        }
        else if (m_XMLType == "Decode")
        {
            // Configuration
            const size_t row = m_XMLDecodeLayoutRow.toInt();
            const size_t column = m_XMLDecodeLayoutColumn.toInt();
            const bool flagRenderingVideo = m_XMLRenderingVideo == "Enable" ? true : false;
            const bool flagDecodeVideo = m_XMLDecodeVideo == "Enable" ? true : false;
            const bool flagDownScaling = m_XMLDownScaling == "Enable" ? true : false;
            const int numberOfThread = m_XMLThreadNumber.toInt();
            const bool flagTimingControlEnable = m_XMLTimingControl == "Enable" ? true : false;
            const float rate = (float)m_XMLRate.toDouble();

            // Create
            SimpleMediaPlayerWindow *pSimpleMediaPlayerWindow =
                    new SimpleMediaPlayerWindow(NO_PARENT, row, column,
                                                flagRenderingVideo, flagDecodeVideo, flagDownScaling,
                                                numberOfThread, flagTimingControlEnable, rate,
                                                m_ExportReportPath, m_SaveScreenshotPath,
                                                description);
            pSimpleMediaPlayerWindow->setCursor(Qt::BlankCursor);
            description = "GLMark - " + description + " (Press ESC to quit)";
            pSimpleMediaPlayerWindow->setWindowTitle(description);

            // set file source & play
            pSimpleMediaPlayerWindow->setFileSource("..//resource//test1080p.avi");
            pSimpleMediaPlayerWindow->play();

            pSimpleMediaPlayerWindow->showMaximized();

            // Collect
            m_SimpleMediaPlayerWindows.push_back(pSimpleMediaPlayerWindow);

            QEventLoop eventLoop;

            connect(pSimpleMediaPlayerWindow, SIGNAL(sendUserTerminate()), this, SLOT(clearSimpleMediaPlayerWindows()));
            connect(pSimpleMediaPlayerWindow, SIGNAL(sendUserTerminate()), &eventLoop, SLOT(quit()));

            pSimpleMediaPlayerWindow->setTotalTime(m_XMLDecodeTime.toInt());
            QTimer::singleShot(m_XMLDecodeTime.toInt()*1000-2000, pSimpleMediaPlayerWindow, SLOT(exportReport()));
            QTimer::singleShot(m_XMLDecodeTime.toInt()*1000-1000, pSimpleMediaPlayerWindow, SLOT(quickshot())); // Capture Snapshot
            QTimer::singleShot(m_XMLDecodeTime.toInt()*1000, pSimpleMediaPlayerWindow, SLOT(close()));
            connect(pSimpleMediaPlayerWindow, SIGNAL(sendClosed()), &eventLoop, SLOT(quit()));

            eventLoop.exec();
        }

        GLMARK_LOG_INFO("Finish test conf '" << m_XMLName << "'");
    }
}

void GLMark::on_pushButtonSaveTestConf_clicked()
{
    // prepare test conf
    QString str("<TEST_CONFIGURATION>\n");
    for (TestConfs::iterator it=m_TestConfs.begin(); it!=m_TestConfs.end(); it++)
    {
        str += (*it)->str;
        GLMARK_LOG_INFO("Save test conf '" << (*it)->pItem->text() << "'");
        GLMARK_LOG_DEBUG((*it)->pItem->text() << "->str = " << (*it)->str);
    }
    str += "</TEST_CONFIGURATION>\n";

    // read from file
    QDomDocument doc("GLMark");
    QFile inFile("GLMark.xml");
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if (!doc.setContent(&inFile))
    {
        inFile.close();
        return;
    }
    inFile.close();

    // write back to file
    QFile outFile("GLMark.xml");
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream( &outFile );
    int position = doc.toString().indexOf("<TEST_CONFIGURATION>");
    if (position == -1)
    {
        position = doc.toString().indexOf("<GENERAL_OPTIONS>");
        stream << doc.toString().insert(position, str); // insert test conf to xml
    }
    else
    {
        int n = doc.toString().indexOf("<GENERAL_OPTIONS>") - position;
        stream << doc.toString().replace(position, n, str); // replace with new test conf
    }
    outFile.close();

    showMessageBoxXMLSaved();
}

void GLMark::getTestConf()
{
    // read from file
    QDomDocument doc("GLMark");
    QFile inFile("GLMark.xml");
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    if (!doc.setContent(&inFile))
    {
        inFile.close();
        return;
    }
    inFile.close();

    // extract test conf string
    int count = doc.toString().count("<TEST>");
    int position[10] = {0};
    for (int i=1; i<=count+1; i++)
    {
        position[i] = doc.toString().indexOf("<TEST>", position[i-1]+1);
        if (position[i] == -1)
        {
            position[i] = doc.toString().indexOf("</TEST_CONFIGURATION>");
        }
    }

    for (int i=1; i<=count; i++)
    {
        QString str(QString::fromStdString(
                        std::string(doc.toString().toStdString(), position[i], position[i+1]-position[i])));

        parseXMLStr(str);

        QListWidgetItem* pListWidgetItem = new QListWidgetItem;
        pListWidgetItem->setText(m_XMLName);
        GLMARK_LOG_INFO("Load test conf '" << m_XMLName << "'");
        ui->listWidget->addItem(pListWidgetItem);

        TestConf* pTestConf = new TestConf;
        pTestConf->pItem = pListWidgetItem;
        pTestConf->str = str;
        m_TestConfs.push_back(pTestConf);
    }

    ui->listWidget->setCurrentRow((int)m_TestConfs.size() - 1);
}

void GLMark::on_pushButtonSaveDecodeSample_clicked()
{
    saveConfXML("DECODE_LAYOUT", QString("%1x%2").arg(ui->row_spin_decode->value())
                                                 .arg(ui->column_spin_decode->value())); // int to QString

    saveConfXML("RENDERING_VIDEO", ui->checkBoxRenderingVideo->isChecked() ? "Enable" : "Disable");
    saveConfXML("DECODE_VIDEO", ui->checkBoxDecodeVideo->isChecked() ? "Enable" : "Disable");
    saveConfXML("DOWN_SCALING", ui->checkBoxDownScaling->isChecked() ? "Enable" : "Disable");

    saveConfXML("THREAD_NUMBER", QString("%1").arg(ui->spin_thread_number->value()));

    saveConfXML("TIMING_CONTROL", ui->checkBoxTimingControlEnable->isChecked() ? "Enable" : "Disable");
    saveConfXML("RATE", QString("%1").arg(ui->doubleSpinBox->value())); // double to QString

    showMessageBoxXMLSaved();

    GLMARK_LOG_INFO("Save Decode Sample Configuration");
}


