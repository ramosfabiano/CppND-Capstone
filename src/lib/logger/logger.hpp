#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <array>
#include <thread>
#include <map>
#include <cassert>

namespace logger
{

/*
    Logger.

    Singleton, thread-safe.
*/
class Logger
{
public:

    // enforcing singleton pattern
    void operator=(Logger const&) = delete;
    Logger(Logger const&) = delete;

    // returns the singleton instance
    static Logger &getInstance()
    {
        static Logger _instance;
        return _instance;
    }

    // log messages are accumulated in a stringstream...
    template <class T>
    Logger &operator<<(const T &logMsg)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        *getLogStream() << logMsg;
        return *this;
    }

    // ...until a std::flush or std::endl is encountered
    typedef std::ostream &(*ManipulatorFunction)(std::ostream &);
    Logger &operator<<(ManipulatorFunction manipFunc)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto logStream = getLogStream();
        bool isEndl  = (manipFunc == static_cast<ManipulatorFunction>(std::endl));
        bool isFlush = (manipFunc == static_cast<ManipulatorFunction>(std::flush));
        if (isFlush || isEndl)
        {
            std::cout << currentTimeAsString() << " " << logStream->str();
            if (isEndl)
            {
                std::cout << std::endl << std::flush;
            }
            logStream->str("");
            // we release the stream after each flush/endl
            // as a way to prevent the map from growing indefinitely
            // as threads are created and destroyed
            // (even though the use of a threadpool would prevent this from happening)
            releaseLogStream();
        }
        return *this;
    }

private:
    // mutex to make the logger thread safe
    std::mutex _mutex;

    // allows a per-thread stream
    std::map<std::thread::id, std::unique_ptr<std::stringstream>> _streamMap;

    // private constructor to enforce singleton pattern
    Logger() {}

    // return stringstream of current thread
    inline std::stringstream* getLogStream()
    {
        // should only be called while holding the lock
        assert(_mutex.owns_lock());

        auto threadId = std::this_thread::get_id();
        auto it = _streamMap.find(threadId);
        if(it == _streamMap.end())
        {
            it = _streamMap.insert(std::make_pair(threadId, std::make_unique<std::stringstream>())).first;
        }
        return it->second.get();
    }

    // release stringstream of current thread
    inline void releaseLogStream()
    {
        // should only be called while holding the lock
        assert(_mutex.owns_lock());

        auto threadId = std::this_thread::get_id();
        auto it = _streamMap.find(threadId);
        if(it != _streamMap.end())
        {
            _streamMap.erase(it);
        }
    }

    // return current time in a format suitable for logging
    std::string currentTimeAsString()
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::array<char, std::size("[hh:mm:ss]X")> timeStr;
        std::strftime(timeStr.data(), timeStr.size(), "[%H:%M:%S]", std::localtime(&now));
        return timeStr.data();
    }
};

#define LOGGER() logger::Logger::getInstance()

} // namespace logger
