#ifndef RENDERINGWINDOW_H
#define RENDERINGWINDOW_H

#include <QWidget>
#include <GLVR.h>

namespace Ui {
class RenderingWindow;
}

class RenderingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RenderingWindow(QWidget *parent = 0);
    ~RenderingWindow();

protected:
    void OnInit();
    void OnUpdate();
    void OnUninit();

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);

    virtual void paintEvent(QPaintEvent*);

    virtual void resizeEvent (QResizeEvent * event);

private:
    bool   m_Initialized;

    Ui::RenderingWindow *ui; 
    surveon::glvr::VideoRenderer* m_pVideoRenderer;
    surveon::glvr::VideoPane* m_pVideoPane;
};

#endif // RENDERINGWINDOW_H
