#ifndef DISPLAYFRAME_H
#define DISPLAYFRAME_H

#include <QFrame>

namespace Ui {
class DisplayFrame;
}

class DisplayFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DisplayFrame(QWidget *parent = 0);
    ~DisplayFrame();

private:
    Ui::DisplayFrame *ui;
};

#endif // DISPLAYFRAME_H
