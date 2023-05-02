#pragma once

#include <memory>
#include <thread>
#include <list>
#include <map>
#include <functional>
#include <mutex>
#include <future>
#include <condition_variable>
#include "logger.hpp"


namespace threadpool
{

/*
    ThreadPool

    Manages a pool of worker threads.
*/
class ThreadPool
{
public:
    typedef std::function<void()> Task;

    ThreadPool(int numThreads)
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            auto t = std::make_unique<std::thread>(&ThreadPool::threadLoop, this);
            _threads.emplace_back(std::move(t));
        }
    }

    ~ThreadPool()
    {
        {
            // discard any pending tasks && flags threads to terminate
            std::lock_guard<std::mutex> lock(_tasksMutex);
            _tasks.clear();
            _terminateRequested = true;
        }

        // wake-up all threads
        _tasksConditionalVar.notify_all();

        // wait for threads to finish
        for (auto& thread : _threads)
        {
            if (thread->joinable())
            {
                thread->join();
            }
        }
    }

    // mimics std::thread constructor
    template< class Function, class... Args >
    void addTask (Function&& f, Args&&... args )
    {
        auto task = std::make_shared<std::packaged_task<void()>>(std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(_tasksMutex);
            _tasks.emplace_back([task]()
            {
                (*task)();
            });
        }
        _tasksConditionalVar.notify_one();
    }

private:
    // collection of treads
    std::list<std::unique_ptr<std::thread>> _threads;

    // collection of tasks
    std::mutex _tasksMutex;
    std::condition_variable _tasksConditionalVar;
    std::list<Task> _tasks;

    // termination flag
    bool _terminateRequested{false};

    // thread loop
    void threadLoop()
    {
        LOGGER() << "Thread " << std::this_thread::get_id() << " started" << std::endl;
        while (true)
        {
            Task taskToRun;
            {
                std::unique_lock<std::mutex> lock(_tasksMutex);
                // wait for a task to be added or for the thread to be cancelled
                _tasksConditionalVar.wait(lock, [this] ()
                {
                    return !_tasks.empty() || _terminateRequested;
                });
                if (_terminateRequested)
                {
                    // abort thread
                    LOGGER() << "Thread " << std::this_thread::get_id() << " terminated." << std::endl;
                    return;
                }
                if (_tasks.empty())
                {
                    // keep waiting
                    continue;
                }
                // pick a task to run
                taskToRun = std::move(_tasks.front());
                _tasks.pop_front();
            }
            // run task
            taskToRun();
        }
    }
};

} // namespace threadpool
