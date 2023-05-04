#pragma once

#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "logger.hpp"
#include "baseexception.hpp"


namespace threadpool
{

IMPLEMENT_CUSTOM_EXCEPTION(ThreadPoolException, "ThreadPoolException")

/*
    ThreadPool

    Manages a pool of worker threads.
*/
class ThreadPool
{
public:
    ThreadPool(int numThreads = std::thread::hardware_concurrency()):
        _threads(),
        _terminationRequested(false),
        _tasksMutex(),
        _tasksCondVar(),
        _tasks()
    {
        if (numThreads < 1)
        {
            throw ThreadPoolException("Number of threads must be greater than 0.");
        }
        LOGGER() << "Creating thread pool with " << numThreads << " threads." << std::endl;
        for (size_t i = 0; i < numThreads; ++i)
        {
            auto t = std::make_unique<std::thread>(&ThreadPool::threadLoop, this);
            _threads.emplace_back(std::move(t));
        }
    }

    ~ThreadPool()
    {
        requestTermination();
        for (auto& t : _threads)
        {
            t->join();
        }
    }

    // enqueues a task
    void addTask(std::function<void()> f)
    {
        {
            std::unique_lock<std::mutex> lock(_tasksMutex);
            _tasks.emplace_back(f);
        }
        _tasksCondVar.notify_one();
    }

private:
    // thread control
    std::list<std::unique_ptr<std::thread>> _threads;
    bool _terminationRequested;

    // task control
    std::mutex _tasksMutex;
    std::condition_variable _tasksCondVar;
    std::list<std::function<void()>> _tasks;

    // thread main function
    void threadLoop()
    {
        auto threadId = std::this_thread::get_id();
        LOGGER() << "Thread " << threadId << " started." << std::endl;
        while (true)
        {
            std::function<void()> taskToRun;
            {
                std::unique_lock<std::mutex> lock(_tasksMutex);
                // wait for a task to be added or for the thread to be cancelled
                _tasksCondVar.wait(lock, [this] ()
                {
                    return !_tasks.empty() || _terminationRequested;
                });
                if (_terminationRequested)
                {
                    LOGGER() << "Thread " << threadId << " exiting." << std::endl;
                    return;
                }
                if (_tasks.empty())
                {
                    continue;
                }
                taskToRun = std::move(_tasks.front());
                _tasks.pop_front();
            }
            // run task
            taskToRun();
        }
    }

    // discard any pending tasks && flags threads to terminate
    void requestTermination()
    {
        {
            std::lock_guard<std::mutex> lock(_tasksMutex);
            _tasks.clear();
            _terminationRequested = true;
        }
        // wake-up all threads
        _tasksCondVar.notify_all();
    }
};

} // namespace threadpool
