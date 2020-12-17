#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <GLVR.h>

using namespace surveon::glvr;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    VideoRendererManager::getInstancePtr()->initialize();

    m_pRenderingWindow  = new RenderingWindow(ui->centralWidget);
}

MainWindow::~MainWindow()
{
    VideoRendererManager::getInstancePtr()->shutdown();
    VideoRendererManager::destroyInstance();
    delete ui;    
}

void MainWindow::resizeEvent (QResizeEvent * event)
{
    QRect displayRect =  this->geometry();

    ui->centralWidget->setGeometry(displayRect);

   m_pRenderingWindow->setGeometry(displayRect);

    QMainWindow::resizeEvent(event);
}
