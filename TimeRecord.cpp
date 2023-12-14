#include "timeRecord.h"

std::chrono::microseconds TimeRecord::elapsed{0};

TimeRecord::TimeRecord()
{
    begin = std::chrono::high_resolution_clock::now();
}

TimeRecord::~TimeRecord()
{
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
}
