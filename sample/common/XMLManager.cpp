#include "XMLManager.h"

XMLManager::XMLManager()
{

}

XMLManager::~XMLManager()
{

}

void XMLManager::generateXMLFile()
{
//    QString filename = QFileDialog::getSaveFileName(0, "Save Xml", ".", "Xml files (*.xml)");
    QString filename = "GLMark.xml";


    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("CONF");

    xmlWriter.writeStartElement("RENDERING_SAMPLE");
    xmlWriter.writeTextElement("LAYOUT",            "1x1");
    xmlWriter.writeTextElement("PIXEL_FORMAT",      "YV12");
    xmlWriter.writeTextElement("VIDEO_RESOLUTION",  "1080p");
    xmlWriter.writeTextElement("SCREEN_RESOLUTION", "1920x1080");
    xmlWriter.writeTextElement("SCREEN_FREQUENCY",  "60Hz");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("DECODE_SAMPLE");
    xmlWriter.writeTextElement("DECODE_LAYOUT",   "1x1");
    xmlWriter.writeTextElement("RENDERING_VIDEO", "Enable");
    xmlWriter.writeTextElement("DECODE_VIDEO",    "Enable");
    xmlWriter.writeTextElement("DOWN_SCALING",    "Enable");
    xmlWriter.writeTextElement("THREAD_NUMBER",   "4");
    xmlWriter.writeTextElement("TIMING_CONTROL",  "Enable");
    xmlWriter.writeTextElement("RATE",            "1.3");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("GENERAL_OPTIONS");
    xmlWriter.writeTextElement("EXPORT_REPORT_PATH",   QDir::currentPath());
    xmlWriter.writeTextElement("SAVE_SCREENSHOT_PATH", QDir::currentPath());
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();

    file.close();
}

void XMLManager::readXMLFile()
{
//    QString filename = QFileDialog::getOpenFileName(0, "Open Xml", ".", "Xml files (*.xml)");
    QString filename = "GLMark.xml";


    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        generateXMLFile();
        file.open(QFile::ReadOnly | QFile::Text);
    }

    m_Rxml.setDevice(&file);
    m_Rxml.readNext();

    while(!m_Rxml.atEnd())
    {
        if(m_Rxml.isStartElement())
        {
            if(m_Rxml.name() == "CONF")
            {
                m_Rxml.readNext();
            }
            else if(m_Rxml.name() == "RENDERING_SAMPLE")
            {
                while(!m_Rxml.atEnd())
                {
                    if(m_Rxml.isEndElement())
                    {
                        m_Rxml.readNext();
                        break;
                    }
                    else if(m_Rxml.isCharacters())
                    {
                        m_Rxml.readNext();
                    }
                    else if(m_Rxml.isStartElement())
                    {
                        if(m_Rxml.name() == "LAYOUT")
                        {
                            QStringList list = m_Rxml.readElementText().split("x");
                            m_LayoutRow = list.at(0);
                            m_LayoutColumn = list.at(1);
                        }
                        else if(m_Rxml.name() == "PIXEL_FORMAT")
                        {
                            m_PixelFormat = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "VIDEO_RESOLUTION")
                        {
                            m_VideoResolution = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "SCREEN_RESOLUTION")
                        {
                            QStringList list = m_Rxml.readElementText().split("x");
                            m_ScreenResolutionWidth = list.at(0);
                            m_ScreenResolutionHeight = list.at(1);
                        }
                        else if(m_Rxml.name() == "SCREEN_FREQUENCY")
                        {
                            QStringList list = m_Rxml.readElementText().split("Hz");
                            m_ScreenFrequency = list.at(0);
                        }
                        m_Rxml.readNext();
                        m_Rxml.readNext();
                    }
                }
            }
            else if(m_Rxml.name() == "DECODE_SAMPLE")
            {
                while(!m_Rxml.atEnd())
                {
                    if(m_Rxml.isEndElement())
                    {
                        m_Rxml.readNext();
                        break;
                    }
                    else if(m_Rxml.isCharacters())
                    {
                        m_Rxml.readNext();
                    }
                    else if(m_Rxml.isStartElement())
                    {
                        if(m_Rxml.name() == "DECODE_LAYOUT")
                        {
                            QStringList list = m_Rxml.readElementText().split("x");
                            m_DecodeLayoutRow = list.at(0);
                            m_DecodeLayoutColumn = list.at(1);
                        }
                        else if(m_Rxml.name() == "RENDERING_VIDEO")
                        {
                            m_RenderingVideo = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "DECODE_VIDEO")
                        {
                            m_DecodeVideo = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "DOWN_SCALING")
                        {
                            m_DownScaling = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "THREAD_NUMBER")
                        {
                            m_ThreadNumber = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "TIMING_CONTROL")
                        {
                            m_TimingControl = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "RATE")
                        {
                            m_Rate = m_Rxml.readElementText();
                        }
                        m_Rxml.readNext();
                        m_Rxml.readNext();
                    }
                }
            }
            else if(m_Rxml.name() == "GENERAL_OPTIONS")
            {
                while(!m_Rxml.atEnd())
                {
                    if(m_Rxml.isEndElement())
                    {
                        m_Rxml.readNext();
                        break;
                    }
                    else if(m_Rxml.isCharacters())
                    {
                        m_Rxml.readNext();
                    }
                    else if(m_Rxml.isStartElement())
                    {
                        if(m_Rxml.name() == "EXPORT_REPORT_PATH")
                        {
                            m_ExportReportPath = m_Rxml.readElementText();
                        }
                        else if(m_Rxml.name() == "SAVE_SCREENSHOT_PATH")
                        {
                            m_SaveScreenshotPath = m_Rxml.readElementText();
                        }
                        m_Rxml.readNext();
                        m_Rxml.readNext();
                    }
                }
            }
            /*else if(m_Rxml.name() == "TEST_CONFIGURATION")
            {
                while(!m_Rxml.atEnd())
                {
                    if(m_Rxml.isEndElement())
                    {
                        m_Rxml.readNext();
                        break;
                    }
                    else if(m_Rxml.isCharacters())
                    {
                        m_Rxml.readNext();
                    }
                    else if(m_Rxml.isStartElement())
                    {
                        if(m_Rxml.name() == "TEST")
                        {
                            while(!m_Rxml.atEnd())
                            {
                                if(m_Rxml.isEndElement())
                                {
                                    m_Rxml.readNext();
                                    break;
                                }
                                else if(m_Rxml.isCharacters())
                                {
                                    m_Rxml.readNext();
                                }
                                else if(m_Rxml.isStartElement())
                                {
                                    if(m_Rxml.name() == "NAME")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "MODE")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "LAYOUT")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "PIXEL_FORMAT")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "VIDEO_RESOLUTION")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "SCREEN_RESOLUTION")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "SCREEN_FREQUENCY")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "TIME")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    else if(m_Rxml.name() == "PROCESS_SETTING")
                                    {
                                        std::cout << m_Rxml.readElementText() << std::endl;
                                    }
                                    m_Rxml.readNext();
                                    m_Rxml.readNext();
                                }
                            }
                        }
                        m_Rxml.readNext();
                    }
                }
            }*/
            else
            {
                m_Rxml.readNext();
            }
        }
        else
        {
            m_Rxml.readNext();
        }
    }

    file.close();
}
