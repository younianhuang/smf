#ifndef VIEWPANE_H
#define VIEWPANE_H

#include "CPUUsage.h"
#include "MemoryUsage.h"
#include "CommonConfig.h"

using surveon::mf::String;

class ViewPane
{
public:
    enum Resolution
    {
        RES_256x256,
        RES_CIF,
        RES_512x512,
        RES_D1,
        RES_720p,
        RES_1080p,
        RES_COUNT
    };

public:
    ViewPane();
    ~ViewPane();

    String getOperatingSystem();
    String getOpenGLVersion() const;
    String getShaderVersion() const;
    String getCPUModel();
    String getGPUModel() const;

    double getProcessCPU();
    double getTotalCPU();

    int getMemoryUsed();

    int getNumProcessor() const
    {
        return m_CPUUsage.getNumProcessors();
    }

    void getGLInfo()
    {
        m_GLInfo = surveon::glvr::VideoRendererManager::getInstance().getCapabilities();
    }

private:
    surveon::glvr::Capabilities m_GLInfo;

    CPUUsage m_CPUUsage;
    MemoryUsage m_MemoryUsage;
};

#endif // VIEWPANE_H

