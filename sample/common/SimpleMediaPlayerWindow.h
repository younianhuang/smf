#ifndef SIMPLEMEDIAPLAYERWINDOW_H
#define SIMPLEMEDIAPLAYERWINDOW_H

#include <QMainWindow>
#include "ViewTile.h"
#include "CommonConfig.h"
#include "ViewPane.h"

namespace Ui {
class SimpleMediaPlayerWindow;
}

class SimpleMediaPlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleMediaPlayerWindow(QWidget *parent = 0, size_t row = 1, size_t column = 1,
                                     bool flagRenderingVideo = true, bool flagDecodeVideo = true,
                                     bool flagDownScaling = true, int numberOfThread = 4,
                                     bool flagTimingControl = true, float rate = 1.3f,
                                     QString exportReportPath = QDir::currentPath(),
                                     QString saveScreenshotPath = QDir::currentPath(),
                                     QString description = "Simple Media Player");
    ~SimpleMediaPlayerWindow();

public:
    void setTotalTime(size_t time);
    void setFileSource(const QString& filename);
    void play(void);
    void enableHardwareDecode(void);
    void disableRenderingVideo(void);
    void disableTimingControl(void);

protected:
    virtual void resizeEvent (QResizeEvent * event);
    virtual void closeEvent ( QCloseEvent * event );
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    // File
    void on_actionOpenFile_triggered();
    void on_actionOpen_Network_Stream_triggered();
    void on_actionExportReport_triggered();
    void on_actionScreenshot_triggered();
    // Window
    void on_actionChange_Layout_triggered();
    // Control
    void on_action_Play_triggered();
    void on_action_Stop_triggered();
    void on_action_Pause_triggered();
    // Options
    void on_actionRendering_Video_triggered(bool checked);
    void on_actionDecode_Video_triggered(bool checked);
    void on_actionDown_Scaling_triggered(bool checked);
    void on_actionThread_Number_triggered();
    void on_actionTimingControl_triggered(bool timingControl);
    void on_actionSetRate_triggered();
    void on_action_Hardware_Decode_triggered(bool checked);

    void updateStatistics();
    void validateSize();
    void quickshot();
    void exportReport(bool showMessageBox = false);

signals:
    void sendUserTerminate();
    void sendClosed();

private:
    void createViewTiles();
    void setupLayout();
    void restart();
    void setNumberOfThread();
    void setRate();
    void setUserTerminate();
    void reinit();
    void closeFile(void);
    QString addHTMLTr(bool alt, QString tag, QString text);

private:
    Ui::SimpleMediaPlayerWindow *ui;

    bool m_RenderingVideo;
    bool m_DecodeVideo;
    bool m_DownScaling;
    bool m_TimingControl;
    bool m_HardwareDecode;
    bool m_Play;
    bool m_Stop;
    int m_NumberOfThread;
    size_t m_Column;
    size_t m_Row;
    size_t m_TimeCounter;
    size_t m_TotalTime;
    float m_Rate;
    float m_SumCPU;
    float m_Last10sCPU;
    float m_SumRendererFPS;
    float m_SumDecoderFPS;
    float m_SumSourceFPS;
    float m_Last10sRendererFPS;
    float m_Last10sDecoderFPS;
    float m_Last10sSourceFPS;
    float m_SumDecoderBandwidth;
    float m_SumSourceBandwidth;
    float m_Last10sDecoderBandwidth;
    float m_Last10sSourceBandwidth;

    QLabel* m_pMsgBar;
    typedef std::list<QLabel*> Labels;
    Labels m_Labels;
    QLineEdit* m_pAskRow;
    QLineEdit* m_pAskColumn;
    QTimer* m_pTimer;
    QString m_Description;
    QString m_ExportReportPath;
    QString m_SaveScreenshotPath;
    QString m_FileSource;

    typedef std::list<ViewTile*> ViewTiles;
    ViewTiles m_ViewTiles;
    ViewPane m_ViewPane;
};

#endif // SIMPLEMEDIAPLAYERWINDOW_H
