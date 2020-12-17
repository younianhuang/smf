#ifndef MEMORYUSAGE_H
#define MEMORYUSAGE_H

#include "CommonConfig.h"

class MemoryUsage
{
public:
    MemoryUsage();
    ~MemoryUsage();

    int getMemoryUsed();

private:
    void updateMemoryUsed();
    int parseLine(char* line);

private:
    int m_MemoryUsed;
};

#endif // MEMORYUSAGE_H

