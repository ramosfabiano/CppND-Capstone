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
            std::lock_guard<std::mutex> lock(_taskMutex);

            // discard any pending tasks
            _tasks.clear();

            // flags threads to terminate
            _terminateRequested = true;
        }

        // notify _all_ threads
        _taskCond.notify_all();

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
            std::unique_lock<std::mutex> lock(_taskMutex);
            _tasks.emplace_back([task]()
            {
                (*task)();
            });
        }
        _taskCond.notify_one();
    }

private:

    // collection of treads
    std::list<std::unique_ptr<std::thread>> _threads;

    // collection of tasks
    std::mutex _taskMutex;
    std::condition_variable _taskCond;
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
                std::unique_lock<std::mutex> lock(_taskMutex);
                // wait for a task to be added or for the thread to be cancelled
                _taskCond.wait(lock, [this] ()
                {
                    return !_tasks.empty() || _terminateRequested;
                });
                if (_terminateRequested)
                {
                    // abort thread
                    LOGGER() << "Thread " << std::this_thread::get_id() << " terminated." << std::endl;
                    return;
                }
                else if (_tasks.empty())
                {
                    // keep waiting
                    continue;
                }
                else
                {
                    // pick a task to run
                    taskToRun = std::move(_tasks.front());
                    _tasks.pop_front();
                }
            }
            // run task
            taskToRun();
        }
    }

};

} // namespace threadpool
