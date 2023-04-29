#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

namespace logger
{

/*
    Logger.
    
    Usage:
        LOGGER() << "Hello world!" << std::endl;
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
        _logStream << logMsg;
        return *this;
    }

    // ...until a std::flush or std::endl is encountered
    typedef std::ostream &(*ManipulatorFunction)(std::ostream &);
    Logger &operator<<(ManipulatorFunction manip)
    {
        bool  isFlush = manip == static_cast<ManipulatorFunction>(std::flush);
        bool  isEndl  = manip == static_cast<ManipulatorFunction>(std::endl);
        if (isFlush || isEndl)
        {
            std::cout << currentTimeAsString() << " " << _logStream.str();
            if (isEndl)
            {
                std::cout << std::endl;
            }
            _logStream.str("");
        }
        return *this;
    }

private:

    std::stringstream _logStream;

    // private constructor to enforce singleton pattern
    Logger() {}

    // return current time in a format suitable for logging
    std::string currentTimeAsString()
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char timeStr[std::size("[hh:mm:ss]X")];
        std::strftime(&timeStr[0], sizeof(timeStr), "[%H:%M:%S]", std::localtime(&now));
        return timeStr;
    }
};

#define LOGGER() logger::Logger::getInstance()

} // namespace logger