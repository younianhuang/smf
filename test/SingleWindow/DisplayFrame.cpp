#include "DisplayFrame.h"
#include "ui_DisplayFrame.h"

DisplayFrame::DisplayFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DisplayFrame)
{
    ui->setupUi(this);
}

DisplayFrame::~DisplayFrame()
{
    delete ui;
}
