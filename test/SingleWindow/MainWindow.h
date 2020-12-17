#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RenderingWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     virtual void resizeEvent (QResizeEvent * event);
private:
    Ui::MainWindow *ui;
    RenderingWindow* m_pRenderingWindow;
};

#endif // MAINWINDOW_H
