#include "ViewPane.h"

ViewPane::ViewPane()
{

}

ViewPane::~ViewPane()
{

}

String ViewPane::getOperatingSystem()
{
    String OperatingSystem = (QSysInfo::productType() + " " + QSysInfo::productVersion()).toStdString();

    OperatingSystem[0] = toupper(OperatingSystem[0]); // first capital

    return OperatingSystem;
}

String ViewPane::getOpenGLVersion() const
{
    return m_GLInfo.glVersion;
}

String ViewPane::getShaderVersion() const
{
    return m_GLInfo.shaderVersion;
}

String ViewPane::getCPUModel()
{

#ifdef SMF_PLATFORM_WIN32

    char CPUBrandString[0x40];
    int CPUInfo[4] = {-1};
    unsigned nExIds;

      // gets the number of valid extended IDs.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    memset(CPUBrandString, 0, sizeof(CPUBrandString));

      // gets cpu brand string
    if (nExIds >= 0x80000004)
    {
        __cpuid(CPUInfo, 0x80000002);
        memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        __cpuid(CPUInfo, 0x80000003);
        memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        __cpuid(CPUInfo, 0x80000004);
        memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));

        // strip spaces before the first character
        String CPUModel(CPUBrandString);
        size_t pos = CPUModel.find_first_not_of(' ');
        CPUModel.assign(CPUModel.c_str()+pos, CPUModel.length()-pos);

        return CPUModel;
    }
    else
    {
        return "Invalid";
    }

#elif defined(SMF_PLATFORM_LINUX)

    char CPUBrandString[0x40];

    FILE* fd = fopen("/proc/cpuinfo", "r");

    // Match substring "model name"
    while (1)
    {
        fgets(CPUBrandString, 0x40, fd);

        // Found
        if (strstr(CPUBrandString, "model name") != NULL)
        {
            // Strip string before ":"
            String CPUModel(CPUBrandString);
            size_t pos = CPUModel.find_first_of(':');
            CPUModel.assign(CPUModel.c_str()+pos+1, CPUModel.length()-pos-1-1); // strip ':' & '\n' also

            // Strip spaces before the first character
            pos = CPUModel.find_first_not_of(' ');
            CPUModel.assign(CPUModel.c_str()+pos, CPUModel.length()-pos);

            fclose(fd);

            return CPUModel;
        }
        else if (feof(fd)) // if not found && end of file
        {
            fclose(fd);

            return "Invalid";
        }

    }

#endif // SMF_PLATFORM_WIN32

}

String ViewPane::getGPUModel() const
{
    return m_GLInfo.gpuModel;
}

double ViewPane::getProcessCPU()
{
    return m_CPUUsage.getProcessCPU();
}

double ViewPane::getTotalCPU()
{
    return m_CPUUsage.getTotalCPU();
}

int ViewPane::getMemoryUsed()
{
    return m_MemoryUsage.getMemoryUsed();
}




