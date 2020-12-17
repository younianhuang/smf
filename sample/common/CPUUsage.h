#ifndef CPUUSAGE_H
#define CPUUSAGE_H

#include "CommonConfig.h"

class CPUUsage
{
public:
    CPUUsage();
    ~CPUUsage();

    double getProcessCPU();
    double getTotalCPU();

    int getNumProcessors() const
    {
        return m_NumProcessors;
    }

private:
    void initProcessCPU();
    void initTotalCPU();

private:
    int m_NumProcessors;
    CPU_TIME_TYPE m_LastCPU;
    CPU_TIME_TYPE m_LastSysCPU;
    CPU_TIME_TYPE m_LastUserCPU;

#ifdef SMF_PLATFORM_WIN32

    HANDLE m_Self;
    PDH_HQUERY m_CpuQuery;
    PDH_HCOUNTER m_CpuTotal;

#elif defined(SMF_PLATFORM_LINUX)

    unsigned int m_LastWork;
    unsigned int m_LastTotal;

#endif
};

#endif // CPUUSAGE_H

