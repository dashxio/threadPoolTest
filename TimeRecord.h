#pragma once

#include <chrono>

class TimeRecord
{
public:
    TimeRecord();
    ~TimeRecord();
    static auto getLastRunTime(){
        return elapsed.count();
    };

private:
    std::chrono::steady_clock::time_point begin, end;
    static std::chrono::microseconds elapsed;    
};
