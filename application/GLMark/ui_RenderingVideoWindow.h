/********************************************************************************
** Form generated from reading UI file 'RenderingVideoWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERINGVIDEOWINDOW_H
#define UI_RENDERINGVIDEOWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RenderingVideoWindow
{
public:
    QAction *actionCreatePIP;
    QAction *actionChangeLayout;
    QAction *actionRGB32;
    QAction *actionYUV420;
    QAction *actionYV12;
    QAction *actionFPS;
    QAction *actionCPU;
    QAction *actionMem;
    QAction *actionCreateRectangle;
    QAction *actionCreatePolygon;
    QAction *actionCreateSprite;
    QAction *actionDestroyRenderable;
    QAction *actionNewWindow;
    QAction *actionCIF;
    QAction *actionD1;
    QAction *action720p;
    QAction *action1080p;
    QAction *actionBandwidth;
    QAction *actionOpenGL;
    QAction *actionSystem;
    QAction *actionExportReport;
    QAction *actionCreateButton;
    QAction *actionDestroyButton;
    QAction *actionShowButtons;
    QAction *actionHideButton;
    QAction *actionScreenshot;
    QAction *action256x256;
    QAction *action512x512;
    QAction *actionEnableCanvas;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuOptions;
    QMenu *menuFormat;
    QMenu *menuStatistics;
    QMenu *menuResolution;
    QMenu *menuInfo;
    QMenu *menuWindow;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RenderingVideoWindow)
    {
        if (RenderingVideoWindow->objectName().isEmpty())
            RenderingVideoWindow->setObjectName(QStringLiteral("RenderingVideoWindow"));
        RenderingVideoWindow->resize(800, 600);
        actionCreatePIP = new QAction(RenderingVideoWindow);
        actionCreatePIP->setObjectName(QStringLiteral("actionCreatePIP"));
        actionChangeLayout = new QAction(RenderingVideoWindow);
        actionChangeLayout->setObjectName(QStringLiteral("actionChangeLayout"));
        actionRGB32 = new QAction(RenderingVideoWindow);
        actionRGB32->setObjectName(QStringLiteral("actionRGB32"));
        actionRGB32->setCheckable(true);
        actionRGB32->setChecked(false);
        actionYUV420 = new QAction(RenderingVideoWindow);
        actionYUV420->setObjectName(QStringLiteral("actionYUV420"));
        actionYUV420->setCheckable(true);
        actionYV12 = new QAction(RenderingVideoWindow);
        actionYV12->setObjectName(QStringLiteral("actionYV12"));
        actionYV12->setCheckable(true);
        actionYV12->setChecked(true);
        actionFPS = new QAction(RenderingVideoWindow);
        actionFPS->setObjectName(QStringLiteral("actionFPS"));
        actionFPS->setCheckable(true);
        actionFPS->setChecked(true);
        actionCPU = new QAction(RenderingVideoWindow);
        actionCPU->setObjectName(QStringLiteral("actionCPU"));
        actionCPU->setCheckable(true);
        actionCPU->setChecked(true);
        actionMem = new QAction(RenderingVideoWindow);
        actionMem->setObjectName(QStringLiteral("actionMem"));
        actionMem->setCheckable(true);
        actionMem->setChecked(true);
        actionCreateRectangle = new QAction(RenderingVideoWindow);
        actionCreateRectangle->setObjectName(QStringLiteral("actionCreateRectangle"));
        actionCreatePolygon = new QAction(RenderingVideoWindow);
        actionCreatePolygon->setObjectName(QStringLiteral("actionCreatePolygon"));
        actionCreateSprite = new QAction(RenderingVideoWindow);
        actionCreateSprite->setObjectName(QStringLiteral("actionCreateSprite"));
        actionDestroyRenderable = new QAction(RenderingVideoWindow);
        actionDestroyRenderable->setObjectName(QStringLiteral("actionDestroyRenderable"));
        actionNewWindow = new QAction(RenderingVideoWindow);
        actionNewWindow->setObjectName(QStringLiteral("actionNewWindow"));
        actionCIF = new QAction(RenderingVideoWindow);
        actionCIF->setObjectName(QStringLiteral("actionCIF"));
        actionCIF->setCheckable(true);
        actionD1 = new QAction(RenderingVideoWindow);
        actionD1->setObjectName(QStringLiteral("actionD1"));
        actionD1->setCheckable(true);
        action720p = new QAction(RenderingVideoWindow);
        action720p->setObjectName(QStringLiteral("action720p"));
        action720p->setCheckable(true);
        action1080p = new QAction(RenderingVideoWindow);
        action1080p->setObjectName(QStringLiteral("action1080p"));
        action1080p->setCheckable(true);
        action1080p->setChecked(true);
        actionBandwidth = new QAction(RenderingVideoWindow);
        actionBandwidth->setObjectName(QStringLiteral("actionBandwidth"));
        actionBandwidth->setCheckable(true);
        actionOpenGL = new QAction(RenderingVideoWindow);
        actionOpenGL->setObjectName(QStringLiteral("actionOpenGL"));
        actionSystem = new QAction(RenderingVideoWindow);
        actionSystem->setObjectName(QStringLiteral("actionSystem"));
        actionExportReport = new QAction(RenderingVideoWindow);
        actionExportReport->setObjectName(QStringLiteral("actionExportReport"));
        actionCreateButton = new QAction(RenderingVideoWindow);
        actionCreateButton->setObjectName(QStringLiteral("actionCreateButton"));
        actionDestroyButton = new QAction(RenderingVideoWindow);
        actionDestroyButton->setObjectName(QStringLiteral("actionDestroyButton"));
        actionShowButtons = new QAction(RenderingVideoWindow);
        actionShowButtons->setObjectName(QStringLiteral("actionShowButtons"));
        actionHideButton = new QAction(RenderingVideoWindow);
        actionHideButton->setObjectName(QStringLiteral("actionHideButton"));
        actionScreenshot = new QAction(RenderingVideoWindow);
        actionScreenshot->setObjectName(QStringLiteral("actionScreenshot"));
        action256x256 = new QAction(RenderingVideoWindow);
        action256x256->setObjectName(QStringLiteral("action256x256"));
        action256x256->setCheckable(true);
        action512x512 = new QAction(RenderingVideoWindow);
        action512x512->setObjectName(QStringLiteral("action512x512"));
        action512x512->setCheckable(true);
        actionEnableCanvas = new QAction(RenderingVideoWindow);
        actionEnableCanvas->setObjectName(QStringLiteral("actionEnableCanvas"));
        actionEnableCanvas->setCheckable(true);
        centralWidget = new QWidget(RenderingVideoWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        RenderingVideoWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RenderingVideoWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 25));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuFormat = new QMenu(menuBar);
        menuFormat->setObjectName(QStringLiteral("menuFormat"));
        menuStatistics = new QMenu(menuBar);
        menuStatistics->setObjectName(QStringLiteral("menuStatistics"));
        menuResolution = new QMenu(menuBar);
        menuResolution->setObjectName(QStringLiteral("menuResolution"));
        menuInfo = new QMenu(menuBar);
        menuInfo->setObjectName(QStringLiteral("menuInfo"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QStringLiteral("menuWindow"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        RenderingVideoWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(RenderingVideoWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RenderingVideoWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuFormat->menuAction());
        menuBar->addAction(menuStatistics->menuAction());
        menuBar->addAction(menuResolution->menuAction());
        menuBar->addAction(menuInfo->menuAction());
        menuOptions->addAction(actionCreatePIP);
        menuOptions->addAction(actionCreateRectangle);
        menuOptions->addAction(actionCreatePolygon);
        menuOptions->addAction(actionCreateSprite);
        menuOptions->addAction(actionDestroyRenderable);
        menuOptions->addSeparator();
        menuOptions->addAction(actionCreateButton);
        menuOptions->addAction(actionDestroyButton);
        menuOptions->addAction(actionShowButtons);
        menuOptions->addAction(actionHideButton);
        menuOptions->addSeparator();
        menuOptions->addAction(actionEnableCanvas);
        menuFormat->addAction(actionRGB32);
        menuFormat->addAction(actionYUV420);
        menuFormat->addAction(actionYV12);
        menuStatistics->addAction(actionCPU);
        menuStatistics->addAction(actionMem);
        menuStatistics->addAction(actionFPS);
        menuStatistics->addAction(actionBandwidth);
        menuResolution->addAction(action256x256);
        menuResolution->addAction(actionCIF);
        menuResolution->addAction(action512x512);
        menuResolution->addAction(actionD1);
        menuResolution->addAction(action720p);
        menuResolution->addAction(action1080p);
        menuInfo->addAction(actionOpenGL);
        menuInfo->addAction(actionSystem);
        menuWindow->addAction(actionNewWindow);
        menuWindow->addAction(actionChangeLayout);
        menuFile->addAction(actionExportReport);
        menuFile->addAction(actionScreenshot);

        retranslateUi(RenderingVideoWindow);

        QMetaObject::connectSlotsByName(RenderingVideoWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RenderingVideoWindow)
    {
        RenderingVideoWindow->setWindowTitle(QApplication::translate("RenderingVideoWindow", "General Test", 0));
        actionCreatePIP->setText(QApplication::translate("RenderingVideoWindow", "Create PIP", 0));
        actionChangeLayout->setText(QApplication::translate("RenderingVideoWindow", "Change Layout", 0));
        actionRGB32->setText(QApplication::translate("RenderingVideoWindow", "RGB32", 0));
        actionYUV420->setText(QApplication::translate("RenderingVideoWindow", "YUV420", 0));
        actionYV12->setText(QApplication::translate("RenderingVideoWindow", "YV12", 0));
        actionFPS->setText(QApplication::translate("RenderingVideoWindow", "FPS", 0));
        actionCPU->setText(QApplication::translate("RenderingVideoWindow", "CPU", 0));
        actionMem->setText(QApplication::translate("RenderingVideoWindow", "Memory", 0));
        actionCreateRectangle->setText(QApplication::translate("RenderingVideoWindow", "Create Rectangle", 0));
        actionCreatePolygon->setText(QApplication::translate("RenderingVideoWindow", "Create Polygon", 0));
        actionCreateSprite->setText(QApplication::translate("RenderingVideoWindow", "Create Sprite", 0));
        actionDestroyRenderable->setText(QApplication::translate("RenderingVideoWindow", "Destroy Renderable", 0));
        actionNewWindow->setText(QApplication::translate("RenderingVideoWindow", "New Window", 0));
        actionCIF->setText(QApplication::translate("RenderingVideoWindow", "CIF", 0));
        actionD1->setText(QApplication::translate("RenderingVideoWindow", "D1", 0));
        action720p->setText(QApplication::translate("RenderingVideoWindow", "720p", 0));
        action1080p->setText(QApplication::translate("RenderingVideoWindow", "1080p", 0));
        actionBandwidth->setText(QApplication::translate("RenderingVideoWindow", "Bandwidth", 0));
        actionOpenGL->setText(QApplication::translate("RenderingVideoWindow", "OpenGL", 0));
        actionSystem->setText(QApplication::translate("RenderingVideoWindow", "System", 0));
        actionExportReport->setText(QApplication::translate("RenderingVideoWindow", "Export Report", 0));
        actionCreateButton->setText(QApplication::translate("RenderingVideoWindow", "Create Button", 0));
        actionDestroyButton->setText(QApplication::translate("RenderingVideoWindow", "Destroy Button", 0));
        actionShowButtons->setText(QApplication::translate("RenderingVideoWindow", "Show Buttons", 0));
        actionHideButton->setText(QApplication::translate("RenderingVideoWindow", "Hide Button", 0));
        actionScreenshot->setText(QApplication::translate("RenderingVideoWindow", "Screenshot", 0));
        action256x256->setText(QApplication::translate("RenderingVideoWindow", "256 x 256", 0));
        action512x512->setText(QApplication::translate("RenderingVideoWindow", "512 x 512", 0));
        actionEnableCanvas->setText(QApplication::translate("RenderingVideoWindow", "Enable Canvas", 0));
        menuOptions->setTitle(QApplication::translate("RenderingVideoWindow", "Options", 0));
        menuFormat->setTitle(QApplication::translate("RenderingVideoWindow", "Format", 0));
        menuStatistics->setTitle(QApplication::translate("RenderingVideoWindow", "Statistics", 0));
        menuResolution->setTitle(QApplication::translate("RenderingVideoWindow", "Resolution", 0));
        menuInfo->setTitle(QApplication::translate("RenderingVideoWindow", "Info", 0));
        menuWindow->setTitle(QApplication::translate("RenderingVideoWindow", "Window", 0));
        menuFile->setTitle(QApplication::translate("RenderingVideoWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class RenderingVideoWindow: public Ui_RenderingVideoWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERINGVIDEOWINDOW_H
