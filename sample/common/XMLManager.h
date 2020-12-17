#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include "CommonConfig.h"

class XMLManager
{
public:
    XMLManager();
    ~XMLManager();

    void readXMLFile();

    QString getLayoutRow() const
    {
        return m_LayoutRow;
    }

    QString getLayoutColumn() const
    {
        return m_LayoutColumn;
    }

    QString getPixelFormat() const
    {
        return m_PixelFormat;
    }

    QString getVideoResolution() const
    {
        return m_VideoResolution;
    }

    QString getScreenResolutionWidth() const
    {
        return m_ScreenResolutionWidth;
    }

    QString getScreenResolutionHeight() const
    {
        return m_ScreenResolutionHeight;
    }

    QString getScreenFrequency() const
    {
        return m_ScreenFrequency;
    }

    QString getExportReportPath() const
    {
        return m_ExportReportPath;
    }

    QString getSaveScreenshotPath() const
    {
        return m_SaveScreenshotPath;
    }

    QString getDecodeLayoutRow() const
    {
        return m_DecodeLayoutRow;
    }

    QString getDecodeLayoutColumn() const
    {
        return m_DecodeLayoutColumn;
    }

    QString getRenderingVideo() const
    {
        return m_RenderingVideo;
    }

    QString getDecodeVideo() const
    {
        return m_DecodeVideo;
    }

    QString getDownScaling() const
    {
        return m_DownScaling;
    }

    QString getThreadNumber() const
    {
        return m_ThreadNumber;
    }

    QString getTimingControl() const
    {
        return m_TimingControl;
    }

    QString getRate() const
    {
        return m_Rate;
    }

private:
    void generateXMLFile();

private:
    QXmlStreamReader m_Rxml;
    // Rendering Sample
    QString m_LayoutRow;
    QString m_LayoutColumn;
    QString m_PixelFormat;
    QString m_VideoResolution;
    QString m_ScreenResolutionWidth;
    QString m_ScreenResolutionHeight;
    QString m_ScreenFrequency;
    // Decode Sample
    QString m_DecodeLayoutRow;
    QString m_DecodeLayoutColumn;
    QString m_RenderingVideo;
    QString m_DecodeVideo;
    QString m_DownScaling;
    QString m_ThreadNumber;
    QString m_TimingControl;
    QString m_Rate;
    // General Options
    QString m_ExportReportPath;
    QString m_SaveScreenshotPath;
};

#endif // XMLMANAGER_H
