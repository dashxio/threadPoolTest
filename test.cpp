#include "ThreadPool.h"
#include "TimeRecord.h"
#include <iostream>
// a task that 1+...+10 000 000;
// the task is divided to 100 tasks: 1+...+100 000,..., 9 900 001+...+10 000 000;
using lint = long long;
const lint max_value = 10000000;

void taskAdd(lint start_value, lint end_value, std::promise<lint> &p)
{
    lint sum = 0;
    for (lint i = start_value; i <= end_value; i++)
    {
        sum += i;
    }
    p.set_value(sum);
}

int main()
{
    std::promise<lint> sum1;
    std::future<lint> sum1_result = sum1.get_future();
    {
        TimeRecord tr;
        std::thread t(taskAdd, 1, max_value, std::ref(sum1));
        t.join();
    }
    std::cout << "Single Thread Compution Result: " << sum1_result.get() << std::endl;
    std::cout << "Single Thread Compution Time: " << TimeRecord::getLastRunTime() << "microseconds.\n";


    std::cout << "----------------------------------------------------------\n";
    std::vector<std::promise<lint>> sums;
    std::vector<std::future<lint>> sums_result;
    lint sum2 = 0;
    for (int i = 0; i < 100; i++)
    {
        sums.push_back(std::promise<lint>{});
        sums_result.push_back(sums[i].get_future());
    }
    {
        TimeRecord tr;
        ThreadPool tp(10);
        for (int i = 0; i < 100; i++)
        {
            lint temp = max_value / 100;
            lint start_value = i * temp + 1;
            lint end_value = (i + 1) * temp;
            tp.addTask(taskAdd, start_value, end_value, std::ref(sums.at(i)));
        }
        for (auto &f : sums_result)
        {
            sum2 += f.get();
        }
    }
    std::cout << "Multi Threads Compution Result: " << sum2 << std::endl;
    std::cout << "Multi Threads Compution Time: " << TimeRecord::getLastRunTime() << "microseconds.\n";
}
