#include "ThreadPool.h"

// if tasks is empty, the thread wait until it is informed
// lock, accquire tasks.size(), if tasks is not empty,

ThreadPool::ThreadPool(int n)
{
    if (n < 0)
        return;

    // create max of worker Threads...
    is_stop = false;
    auto worker = [this]()
    {
        while (1)
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_cv.wait(lock, [this]()
                      { return !m_tasks.empty() || is_stop; });
            if (is_stop && m_tasks.empty())
            {
                return;
            }
            auto task{std::move(m_tasks.front())};
            m_tasks.pop();
            lock.unlock();
            task();
        }
    };

    for (int i = 0; i < n; i++)
    {
        m_workers.push_back(std::thread{worker});
    }
}

ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> lock(m_mtx);
    is_stop = true;
    lock.unlock();
    m_cv.notify_all();
    for (auto &t : m_workers)
    {
        t.join();
    }
}
