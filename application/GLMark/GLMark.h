#ifndef GLMARK_H
#define GLMARK_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QRadioButton>
#include <QComboBox>
#include <QListWidgetItem>
#include <QDomDocument>

#include "ScreenSetter.h"
#include <RenderingVideoWindow.h>
#include "XMLManager.h"
#include <SimpleMediaPlayerWindow.h>

namespace Ui {
class GLMark;
}

class GLMark : public QMainWindow
{
    Q_OBJECT

public:
    struct TestConf
    {
        QListWidgetItem* pItem;
        QString str;
    };

public:
    explicit GLMark(QWidget *parent = 0);
    ~GLMark();

    int exeRunAll();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private slots:
    // Rendering Sample
    void on_pushButtonGeneralTest_clicked();
    void on_pushButtonSaveRenderingSample_clicked();
    // Rendering Benchmark
    void on_pushButtonMultipleWindows_clicked();
    void on_pushButtonTextureUpload_clicked();
    void on_pushButtonDifferentResolution_clicked();
    void on_pushButtonDifferentFormat_clicked();
    void on_pushButtonMultipleMonitors_clicked();
    void on_pushButtonRunAll_clicked();
    // Test Configuration
    void on_pushButtonAdd_clicked();
    void on_radio_mode_texture_toggled(bool checked);
    void on_pushButtonRunTestConf_clicked();
    void on_pushButtonSaveTestConf_clicked();
    // Decode Sample
    void on_pushButtonRunDecodeSample_clicked();
    void on_pushButtonSaveDecodeSample_clicked();
    // Decode Benchmark
    void on_pushButtonMultipleWindows_db_clicked();
    // General Options
    void on_pushButtonExportReport_clicked();
    void on_pushButtonSaveScreenshot_clicked();
    void on_pushButtonSaveGeneralOptions_clicked();

    void updateScreenFrequencyUI(int index);
    void setUserTerminate();
    void clearRenderingVideoWindows();
    void showCurrentItem(int index);
    void openEditor(QListWidgetItem* pItem);
    void deleteCurrentItem();
    void editCurrentItem();
    void moveUpCurrentItem();
    void moveDownCurrentItem();
    void clearSimpleMediaPlayerWindows();

signals:
    void sendUserTerminate();

private:
    void setupScreenConfigUI(QComboBox* pCombobox);
    void runMultipleMonitors(bool showMessageBox = false);
    void saveConfXML(QString tagName, QString text);
    void editConfXML(QDomDocument* pDoc, QString tagName, QString text);
    void clearTestConfs();
    QString getPixelFormatStr();
    QString getVideoResolutionStr();
    void setCurrentItemStr();
    void parseXMLStr(const QString& str);
    inline QString getXMLtext(QDomDocument *pDoc, QString tagName);
    void setTestVideoResolution();
    void setTestPixelFormat();
    void getTestConf();
    void showMessageBoxXMLSaved();
    void keepChangingLayout();
    void demoRenderable();

private:
    typedef std::list<RenderingVideoWindow*> RenderingVideoWindows;
    typedef std::list<TestConf*> TestConfs;
    typedef std::list<SimpleMediaPlayerWindow*> SimpleMediaPlayerWindows;

    Ui::GLMark *ui;

    bool m_bUserTerminate;
    bool m_bRunAll;

    QString m_ExportReportPath;
    QString m_SaveScreenshotPath;
    QString m_XMLMode;
    QString m_XMLLayoutRow;
    QString m_XMLLayoutColumn;
    QString m_XMLPixelFormat;
    QString m_XMLVideoResolution;
    QString m_XMLScreenSizeWidth;
    QString m_XMLScreenSizeHeight;
    QString m_XMLFrequency;
    QString m_XMLProcessSetting;
    QString m_XMLTime;
    QString m_XMLName;
    QString m_XMLType;
    QString m_XMLDecodeLayoutRow;
    QString m_XMLDecodeLayoutColumn;
    QString m_XMLRenderingVideo;
    QString m_XMLDecodeVideo;
    QString m_XMLDownScaling;
    QString m_XMLThreadNumber;
    QString m_XMLTimingControl;
    QString m_XMLRate;
    QString m_XMLDecodeTime;

    ScreenSetter m_ScreenSetter;
    RenderingVideoWindows m_RenderingVideoWindows;
    XMLManager m_XMLManager;
    TestConfs m_TestConfs;
    CPUUsage m_CPUUsage;
    SimpleMediaPlayerWindows m_SimpleMediaPlayerWindows;
};

#endif // GLMARK_H
