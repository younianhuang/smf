#ifndef RENDERING_VIDEO_WINDOW_H
#define RENDERING_VIDEO_WINDOW_H

#include "RenderingWindow.h"
#include "ViewPane.h"

#include <QMainWindow>
#include <QTimer>
#include <QLineEdit>
#include <QDockWidget>

namespace Ui {
class RenderingVideoWindow;
}

class RenderingVideoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RenderingVideoWindow(QWidget *parent = 0, size_t row = 1, size_t column = 1,
                                  surveon::mf::MediaSubType pixelFormat = surveon::mf::SMF_VIDEO_FORMAT_YV12,
                                  ViewPane::Resolution resolution = ViewPane::RES_1080p,
                                  QString exportReportPath = QDir::currentPath(),
                                  QString saveScreenshotPath = QDir::currentPath(),
                                  bool hideMenuBar = false, bool hideStatusBar = false,
                                  bool textureUpload = false, bool showProcessID = false,
                                  QString description = "Rendering Video");
    ~RenderingVideoWindow();

    void setTotalTime(size_t time)
    {
        m_TotalTime = time;
    }

public slots:
    // options
    void on_actionCreatePIP_triggered();
    void on_actionCreateRectangle_triggered();
    void on_actionCreatePolygon_triggered();
    void on_actionCreateSprite_triggered();
    void on_actionDestroyRenderable_triggered();
    void on_actionCreateButton_triggered();
    void on_actionDestroyButton_triggered();
    void on_actionShowButtons_triggered();
    void on_actionHideButton_triggered();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    void createRenderWindows();
    void setupLayout();
    void restart();
    void reinit();

private slots:
    // file
    void on_actionExportReport_triggered();
    void on_actionScreenshot_triggered();
    // window
    void on_actionNewWindow_triggered();
    void on_actionChangeLayout_triggered();
    // format
    void on_actionRGB32_changed();
    void on_actionYUV420_changed();
    void on_actionYV12_changed();
    // resolution
    void on_action256x256_changed();
    void on_actionCIF_changed();
    void on_action512x512_changed();
    void on_actionD1_changed();
    void on_action720p_changed();
    void on_action1080p_changed();
    // info
    void on_actionOpenGL_triggered();
    void on_actionSystem_triggered();

    void updateStatistics();
    void validateSize();

#ifdef Bool
    #undef Bool // "#define Bool Int" in Xlib.h
#endif

    void exportReport(bool showMessageBox = false);
    void activeRenderingVideoWindow();
    void quickshot();

signals:
    void sendUserTerminate();
    void sendClosed();

private:
    void clearOthers(ViewPane::Resolution resolution);
    void clearOthers(surveon::mf::MediaSubType format);
    void setUserTerminate();

private:
    typedef std::list<RenderingWindow*> RenderingWindows;
    Ui::RenderingVideoWindow *ui;

    RenderingWindows m_RenderingWindows;

    bool m_ShowProcessID;
    bool m_TextureUpload;
    size_t m_Row;
    size_t m_Column;
    size_t m_VideoBufferSize;
    size_t m_TimeCounter;
    size_t m_TotalTime;
    size_t m_WindowCount;
    float m_SumCPU;
    float m_Last10sCPU;
    float m_SumFPS;
    float m_Last10sFPS;
    float m_SumBandwidth;
    float m_Last10sBandwidth;

    QTimer* m_pTimer;
    QLineEdit* m_pAskRow;
    QLineEdit* m_pAskColumn;
    QLineEdit* m_pAskVideoBufferSize;
    QString m_Description;
    QString m_ExportReportPath;
    QString m_SaveScreenshotPath;
    QLabel* m_pMsgBar;

    surveon::mf::MediaSubType m_PixelFormat;

    ViewPane::Resolution m_Resolution;
    ViewPane m_ViewPane;
};

#endif // RENDERING_VIDEO_WINDOW_H
