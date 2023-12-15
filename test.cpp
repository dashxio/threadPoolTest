#include "ThreadPool.h"
#include "TimeRecord.h"
#include <iostream>
// a task that 1+...+10 000 000;
// the task is divided to 100 tasks: 1+...+100 000,..., 9 900 001+...+10 000 000;
using lint = long long;
const lint max_value = 10000000;

// last_sum is a promise object that stores the result of the last subtask;
// this_sum is a promise object that stores the result of adding last_sum and the current subtask result.
void taskAdd(std::promise<lint>& last_sum, lint start_value, lint end_value, std::promise<lint> &this_sum)
{
    std::future<lint> last_sum_result = last_sum.get_future();
    lint sum = 0;
    for (lint i = start_value; i <= end_value; i++)
    {
        sum += i;
    }
    this_sum.set_value(sum + last_sum_result.get());
}

int main()
{
    std::promise<lint> sum1;
    std::future<lint> sum1_result = sum1.get_future();
    std::promise<lint> sum0;
    sum0.set_value(0);//last_sum value is zero.
    {
        TimeRecord tr;
        std::thread t([&]{ taskAdd(sum0, 1, max_value, sum1); });//bind sum0
        t.join();
    }
    std::cout << "Single Thread Compution Result: " << sum1_result.get() << std::endl;
    std::cout << "Single Thread Compution Time: " << TimeRecord::getLastRunTime() << "microseconds.\n";


    std::cout << "----------------------------------------------------------\n";
    std::vector<std::promise<lint>> sum_list;//store any stages computation result.
    sum_list.push_back(std::promise<lint>{});
    sum_list.back().set_value(0);
    for (int i = 0; i < 100; i++)
    {
        sum_list.push_back(std::promise<lint>{});
    }
    std::future<lint> &sum2_result = sum_list.back().get_future();//get the final computation result.

    {
        TimeRecord tr;
        ThreadPool tp(10);
        for (int i = 0; i < 100; i++)
        {
            lint temp = max_value / 100;
            lint start_value = i * temp + 1;
            lint end_value = (i + 1) * temp;
            tp.addTask([=, &sum_list]{ taskAdd(sum_list.at(i), start_value, end_value, sum_list.at(i + 1)); });//"start_value, end_value, i" need to be passed by value.
        }
    }
    std::cout << "Multi Threads Compution Result: " << sum2_result.get() << std::endl;
    std::cout << "Multi Threads Compution Time: " << TimeRecord::getLastRunTime() << "microseconds.\n";
}
