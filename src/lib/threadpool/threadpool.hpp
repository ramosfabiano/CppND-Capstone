#pragma once

#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <future>
#include <condition_variable>
#include "logger.hpp"
#include "baseexception.hpp"


namespace threadpool
{

IMPLEMENT_CUSTOM_EXCEPTION(ThreadPoolException, "ThreadPoolException")

/*
    ThreadPool

    Manages a pool of worker threads to handle HTTP requests.
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
        _terminationRequested = true;
        _tasksCondVar.notify_all();

        for (auto& t : _threads)
        {
            t->join();
        }
    }

    // enqueues a task
    std::future<bool> addTask(std::function<bool()> task)
    {
        std::promise<bool> p;
        std::shared_ptr<std::promise<bool>> promise = std::make_shared<std::promise<bool>>();
        {
            std::lock_guard<std::mutex> lock(_tasksMutex);
            _tasks.emplace_back([task, promise]()
            {
                try
                {
                    promise->set_value(std::invoke(task));
                }
                catch (std::exception& e)
                {
                    LOGGER() << "Exception in task: " << e.what() << std::endl;
                    promise->set_value(false);
                }
            });
        }
        _tasksCondVar.notify_one();
        return promise->get_future();
    }

    // clear all unprocessed tasks
    void clearTasks()
    {
        std::lock_guard<std::mutex> lock(_tasksMutex);
        _tasks.clear();
        _tasksCondVar.notify_all();
    }

private:
    // thread control
    std::list<std::unique_ptr<std::thread>> _threads;
    volatile bool _terminationRequested;

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
                    return _terminationRequested || !_tasks.empty();
                });
                // only consider termination after all tasks in queue have been processed
                if (_tasks.empty())
                {
                    if (_terminationRequested)
                    {
                        LOGGER() << "Thread " << threadId << " exiting." << std::endl;
                        return;
                    }
                    continue;
                }
                taskToRun = std::move(_tasks.front());
                _tasks.pop_front();
            }
            // run task
            taskToRun();
        }
    }
};

} // namespace threadpool
