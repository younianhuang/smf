/********************************************************************************
** Form generated from reading UI file 'SimpleMediaPlayerWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLEMEDIAPLAYERWINDOW_H
#define UI_SIMPLEMEDIAPLAYERWINDOW_H

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

class Ui_SimpleMediaPlayerWindow
{
public:
    QAction *actionChange_Layout;
    QAction *actionFPS;
    QAction *actionCPU;
    QAction *actionMemory;
    QAction *actionRendering_Video;
    QAction *actionDecode_Video;
    QAction *actionDown_Scaling;
    QAction *actionThread_Number;
    QAction *actionTimingControl;
    QAction *actionSetRate;
    QAction *actionExportReport;
    QAction *actionScreenshot;
    QAction *actionOpenFile;
    QAction *action_Play;
    QAction *action_Stop;
    QAction *action_Pause;
    QAction *action_Hardware_Decode;
    QAction *actionOpen_Network_Stream;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuWindow;
    QMenu *menuStatistics;
    QMenu *menuOptions;
    QMenu *menuFile;
    QMenu *menu_Control;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SimpleMediaPlayerWindow)
    {
        if (SimpleMediaPlayerWindow->objectName().isEmpty())
            SimpleMediaPlayerWindow->setObjectName(QStringLiteral("SimpleMediaPlayerWindow"));
        SimpleMediaPlayerWindow->resize(800, 600);
        actionChange_Layout = new QAction(SimpleMediaPlayerWindow);
        actionChange_Layout->setObjectName(QStringLiteral("actionChange_Layout"));
        actionFPS = new QAction(SimpleMediaPlayerWindow);
        actionFPS->setObjectName(QStringLiteral("actionFPS"));
        actionFPS->setCheckable(true);
        actionFPS->setChecked(true);
        actionCPU = new QAction(SimpleMediaPlayerWindow);
        actionCPU->setObjectName(QStringLiteral("actionCPU"));
        actionCPU->setCheckable(true);
        actionCPU->setChecked(true);
        actionMemory = new QAction(SimpleMediaPlayerWindow);
        actionMemory->setObjectName(QStringLiteral("actionMemory"));
        actionMemory->setCheckable(true);
        actionMemory->setChecked(true);
        actionRendering_Video = new QAction(SimpleMediaPlayerWindow);
        actionRendering_Video->setObjectName(QStringLiteral("actionRendering_Video"));
        actionRendering_Video->setCheckable(true);
        actionRendering_Video->setChecked(true);
        actionDecode_Video = new QAction(SimpleMediaPlayerWindow);
        actionDecode_Video->setObjectName(QStringLiteral("actionDecode_Video"));
        actionDecode_Video->setCheckable(true);
        actionDecode_Video->setChecked(true);
        actionDown_Scaling = new QAction(SimpleMediaPlayerWindow);
        actionDown_Scaling->setObjectName(QStringLiteral("actionDown_Scaling"));
        actionDown_Scaling->setCheckable(true);
        actionDown_Scaling->setChecked(true);
        actionThread_Number = new QAction(SimpleMediaPlayerWindow);
        actionThread_Number->setObjectName(QStringLiteral("actionThread_Number"));
        actionTimingControl = new QAction(SimpleMediaPlayerWindow);
        actionTimingControl->setObjectName(QStringLiteral("actionTimingControl"));
        actionTimingControl->setCheckable(true);
        actionTimingControl->setChecked(true);
        actionSetRate = new QAction(SimpleMediaPlayerWindow);
        actionSetRate->setObjectName(QStringLiteral("actionSetRate"));
        actionExportReport = new QAction(SimpleMediaPlayerWindow);
        actionExportReport->setObjectName(QStringLiteral("actionExportReport"));
        actionScreenshot = new QAction(SimpleMediaPlayerWindow);
        actionScreenshot->setObjectName(QStringLiteral("actionScreenshot"));
        actionOpenFile = new QAction(SimpleMediaPlayerWindow);
        actionOpenFile->setObjectName(QStringLiteral("actionOpenFile"));
        action_Play = new QAction(SimpleMediaPlayerWindow);
        action_Play->setObjectName(QStringLiteral("action_Play"));
        action_Play->setEnabled(false);
        action_Stop = new QAction(SimpleMediaPlayerWindow);
        action_Stop->setObjectName(QStringLiteral("action_Stop"));
        action_Stop->setEnabled(false);
        action_Pause = new QAction(SimpleMediaPlayerWindow);
        action_Pause->setObjectName(QStringLiteral("action_Pause"));
        action_Pause->setEnabled(false);
        action_Hardware_Decode = new QAction(SimpleMediaPlayerWindow);
        action_Hardware_Decode->setObjectName(QStringLiteral("action_Hardware_Decode"));
        action_Hardware_Decode->setCheckable(true);
        actionOpen_Network_Stream = new QAction(SimpleMediaPlayerWindow);
        actionOpen_Network_Stream->setObjectName(QStringLiteral("actionOpen_Network_Stream"));
        centralWidget = new QWidget(SimpleMediaPlayerWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SimpleMediaPlayerWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SimpleMediaPlayerWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 25));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QStringLiteral("menuWindow"));
        menuStatistics = new QMenu(menuBar);
        menuStatistics->setObjectName(QStringLiteral("menuStatistics"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menu_Control = new QMenu(menuBar);
        menu_Control->setObjectName(QStringLiteral("menu_Control"));
        SimpleMediaPlayerWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(SimpleMediaPlayerWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SimpleMediaPlayerWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menu_Control->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuStatistics->menuAction());
        menuWindow->addAction(actionChange_Layout);
        menuStatistics->addAction(actionCPU);
        menuStatistics->addAction(actionMemory);
        menuStatistics->addAction(actionFPS);
        menuOptions->addAction(actionRendering_Video);
        menuOptions->addAction(actionDecode_Video);
        menuOptions->addAction(actionDown_Scaling);
        menuOptions->addAction(actionThread_Number);
        menuOptions->addAction(actionTimingControl);
        menuOptions->addAction(actionSetRate);
        menuOptions->addAction(action_Hardware_Decode);
        menuFile->addAction(actionOpenFile);
        menuFile->addAction(actionOpen_Network_Stream);
        menuFile->addAction(actionExportReport);
        menuFile->addAction(actionScreenshot);
        menu_Control->addAction(action_Play);
        menu_Control->addAction(action_Stop);
        menu_Control->addAction(action_Pause);

        retranslateUi(SimpleMediaPlayerWindow);

        QMetaObject::connectSlotsByName(SimpleMediaPlayerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SimpleMediaPlayerWindow)
    {
        SimpleMediaPlayerWindow->setWindowTitle(QApplication::translate("SimpleMediaPlayerWindow", "MainWindow", 0));
        actionChange_Layout->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Change Layout", 0));
        actionFPS->setText(QApplication::translate("SimpleMediaPlayerWindow", "FPS", 0));
        actionCPU->setText(QApplication::translate("SimpleMediaPlayerWindow", "CPU", 0));
        actionMemory->setText(QApplication::translate("SimpleMediaPlayerWindow", "Memory", 0));
        actionRendering_Video->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Rendering Video", 0));
        actionDecode_Video->setText(QApplication::translate("SimpleMediaPlayerWindow", "De&code Video", 0));
        actionDown_Scaling->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Down Scaling", 0));
        actionThread_Number->setText(QApplication::translate("SimpleMediaPlayerWindow", "Thread &Number", 0));
        actionTimingControl->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Timing Control", 0));
        actionSetRate->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Set Rate", 0));
        actionExportReport->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Export Report", 0));
        actionScreenshot->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Screenshot", 0));
        actionOpenFile->setText(QApplication::translate("SimpleMediaPlayerWindow", "Open &File...", 0));
        action_Play->setText(QApplication::translate("SimpleMediaPlayerWindow", "P&lay", 0));
        action_Stop->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Stop", 0));
        action_Pause->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Pause", 0));
        action_Hardware_Decode->setText(QApplication::translate("SimpleMediaPlayerWindow", "&Hardware Decode", 0));
        actionOpen_Network_Stream->setText(QApplication::translate("SimpleMediaPlayerWindow", "Open &Network Stream...", 0));
        menuWindow->setTitle(QApplication::translate("SimpleMediaPlayerWindow", "&Window", 0));
        menuStatistics->setTitle(QApplication::translate("SimpleMediaPlayerWindow", "&Statistics", 0));
        menuOptions->setTitle(QApplication::translate("SimpleMediaPlayerWindow", "&Options", 0));
        menuFile->setTitle(QApplication::translate("SimpleMediaPlayerWindow", "&File", 0));
        menu_Control->setTitle(QApplication::translate("SimpleMediaPlayerWindow", "&Control", 0));
    } // retranslateUi

};

namespace Ui {
    class SimpleMediaPlayerWindow: public Ui_SimpleMediaPlayerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLEMEDIAPLAYERWINDOW_H
