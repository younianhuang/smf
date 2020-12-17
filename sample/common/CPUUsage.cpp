#include "CPUUsage.h"

CPUUsage::CPUUsage()
{
    initProcessCPU();
    initTotalCPU();
}

CPUUsage::~CPUUsage()
{

}

#ifdef SMF_PLATFORM_WIN32

double CPUUsage::getProcessCPU()
{
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(m_Self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - m_LastSysCPU.QuadPart) +
        (user.QuadPart - m_LastUserCPU.QuadPart);
    percent /= (now.QuadPart - m_LastCPU.QuadPart);
    percent /= m_NumProcessors;
    m_LastCPU = now;
    m_LastUserCPU = user;
    m_LastSysCPU = sys;

    return percent * 100;
}

double CPUUsage::getTotalCPU()
{
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(m_CpuQuery);
    PdhGetFormattedCounterValue(m_CpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

    return counterVal.doubleValue;
}

void CPUUsage::initProcessCPU()
{
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    m_NumProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&m_LastCPU, &ftime, sizeof(FILETIME));

    m_Self = GetCurrentProcess();
    GetProcessTimes(m_Self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&m_LastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&m_LastUserCPU, &fuser, sizeof(FILETIME));
}

void CPUUsage::initTotalCPU()
{
    PdhOpenQuery(NULL, NULL, &m_CpuQuery);
    PdhAddCounter(m_CpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &m_CpuTotal);
    PdhCollectQueryData(m_CpuQuery);
}

#elif defined(SMF_PLATFORM_LINUX)

void CPUUsage::initProcessCPU()
{
    FILE* file;
    struct tms timeSample;
    char line[128];

    m_LastCPU = times(&timeSample);
    m_LastSysCPU = timeSample.tms_stime;
    m_LastUserCPU = timeSample.tms_utime;

    file = fopen("/proc/cpuinfo", "r");
    m_NumProcessors = 0;
    while(fgets(line, 128, file) != NULL){
        if (strncmp(line, "processor", 9) == 0) m_NumProcessors++;
    }
    fclose(file);
}

void CPUUsage::initTotalCPU()
{
    char str[8];
    unsigned int user, nice, system, idle;
    FILE* pFile = fopen("/proc/stat", "r");

    fscanf(pFile, "%s", str); // "cpu"
    fscanf(pFile, "%d %d %d %d", &user, &nice, &system, &idle);

    fclose(pFile);

    m_LastWork = user + nice + system;
    m_LastTotal = m_LastWork + idle;
}

double CPUUsage::getProcessCPU()
{
    struct tms timeSample;
    clock_t now;
    double percent;

    now = times(&timeSample);
    if (now <= m_LastCPU || timeSample.tms_stime < m_LastSysCPU ||
            timeSample.tms_utime < m_LastUserCPU){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        percent = (timeSample.tms_stime - m_LastSysCPU) +
                (timeSample.tms_utime - m_LastUserCPU);
        percent /= (now - m_LastCPU);
        percent /= m_NumProcessors;
        percent *= 100;
    }
    m_LastCPU = now;
    m_LastSysCPU = timeSample.tms_stime;
    m_LastUserCPU = timeSample.tms_utime;

    return percent;
}

double CPUUsage::getTotalCPU()
{
    char str[8];
    unsigned int user, nice, system, idle, work, total;
    double percent;
    FILE* pFile = fopen("/proc/stat", "r");

    fscanf(pFile, "%s", str); // "cpu"
    fscanf(pFile, "%d %d %d %d", &user, &nice, &system, &idle);

    fclose(pFile);

    work = user + nice + system;
    total = work + idle;
    percent = 100.0 * (work-m_LastWork) / (total-m_LastTotal);

    m_LastWork = work;
    m_LastTotal = total;

    return percent;
}

#endif
