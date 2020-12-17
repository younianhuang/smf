#include "MemoryUsage.h"

MemoryUsage::MemoryUsage() :
    m_MemoryUsed(0)
{

}

MemoryUsage::~MemoryUsage()
{

}

int MemoryUsage::getMemoryUsed()
{
    updateMemoryUsed();

    return m_MemoryUsed;
}

void MemoryUsage::updateMemoryUsed()
{

#ifdef SMF_PLATFORM_WIN32

    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    m_MemoryUsed = pmc.PrivateUsage >> 20;

#elif defined(SMF_PLATFORM_LINUX)

    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    m_MemoryUsed = result >> 10;

#endif

}

int MemoryUsage::parseLine(char* line)
{
    int i = (int)strlen(line);
    while (*line < '0' || *line > '9') line++;
    line[i-3] = '\0';
    i = atoi(line);
    return i;
}

