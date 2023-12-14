#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{

public:
    ThreadPool(int n);
    ~ThreadPool();

    template <typename Func, typename... Args>
    void addTask(Func &&func, Args &&...args)
    {
        std::packaged_task<void()> task{std::bind(std::forward<Func>(func), std::forward<Args>(args)...)};
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_tasks.emplace(std::move(task));
        }
        m_cv.notify_one();
    }

private:
    std::queue<std::packaged_task<void()>> m_tasks;

    std::vector<std::thread> m_workers;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool is_stop;
};
