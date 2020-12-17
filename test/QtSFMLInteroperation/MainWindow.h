#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <vector>
#include "SFMLRenderWindow.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow//, public QThread
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void resizeEvent (QResizeEvent * event);

    virtual void showEvent(QShowEvent* event);

    virtual void closeEvent ( QCloseEvent * event );

    void createRenderWindows();

    void setupLayout();

    void run();

private:
    Ui::MainWindow *ui;
    std::vector<SFMLRenderWindow *> m_RenderWindows ;
    bool m_Run;
    size_t m_Column;
    size_t m_Row;
    sf::Thread m_Thread;
    sf::Font m_Font;

};

#endif // MAINWINDOW_H
