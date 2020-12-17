#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QResizeEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Run(false),
    m_Column(1),
    m_Row(1),
    m_Thread(&MainWindow::run, this)
{
    ui->setupUi(this);

    createRenderWindows();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent*)
{
    if(!m_Run)
    {
        m_Thread.launch();

        m_Run = true;
    }
}

void MainWindow::resizeEvent (QResizeEvent * event)
{             
    QRect displayRect =  this->geometry();

    ui->centralWidget->setGeometry(displayRect);

    setupLayout();

    QMainWindow::resizeEvent(event);
}

void MainWindow::closeEvent ( QCloseEvent * event )
{
    m_Run = false;

    m_Thread.wait();

    int count = m_Column * m_Row;
    for(int i=0; i<count; i++ )
    {
        SFMLRenderWindow* pRenderWindow = m_RenderWindows[i];
        pRenderWindow->Stop();
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::createRenderWindows()
{
       if(m_RenderWindows.empty())
       {

           int count = m_Column * m_Row;
           for(int i=0; i<count; i++ )
           {
               SFMLRenderWindow* pRenderWindow = new SFMLRenderWindow(ui->centralWidget);
               m_RenderWindows.push_back(pRenderWindow);
           }

           if (m_Font.loadFromFile("..//resource//calibri.ttf"))
           {
               for(int i=0; i<count; i++ )
               {
                   m_RenderWindows[i]->setFont(m_Font);
               }
           }
           else
           {
               QMessageBox msgBox;
               msgBox.setText("Load Font failed.");
               msgBox.exec();
           }
    }
}

void MainWindow::setupLayout()
{
    if(m_RenderWindows.empty())
        return;

    QRect displayArea = ui->centralWidget->geometry();

    int tileLeft = 0;
    int tileTop = 0;
    int tileWidth = (displayArea.width() - tileLeft* 2)/m_Column;
    int tileHeight = (displayArea.height() - tileTop*2)/m_Row;

    QRect tileRect(tileLeft, tileTop, tileWidth, tileHeight);

    int index=0;
    for(size_t j = 0; j < m_Row; j ++)
    {
        tileRect.moveTop(tileTop + j * tileHeight);

        for(size_t i = 0; i < m_Column; i ++)
        {
            tileRect.moveLeft(tileLeft + i * tileWidth);

            SFMLRenderWindow* pRenderWindow = m_RenderWindows[index++];
            pRenderWindow->setGeometry(tileRect);
        }
    }
}


void MainWindow::run()
{
    while(m_Run)
    {       
        for(size_t i=0; i<m_RenderWindows.size(); i++ )
        {
            m_RenderWindows[i]->Update();
        }
    }
}
