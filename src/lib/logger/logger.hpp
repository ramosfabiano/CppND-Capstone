#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

namespace logger
{

#define LOGGER() logger::Logger::getInstance()

class Logger
{

public:

    // singleton pattern
    void operator=(Logger const&) = delete;
    Logger(Logger const&) = delete;

    static Logger &getInstance()
    {
        static Logger _instance;
        return _instance;
    }

    // accumulates regular log messages...
    template <class T>
    Logger &operator<<(const T &logMsg)
    {
        _logStream << logMsg;
        return *this;
    }

    // ... until a flush or endl is encountered
    typedef std::ostream &(*ManipulatorFunction)(std::ostream &);
    Logger &operator<<(ManipulatorFunction manip)
    {
        if (manip == static_cast<ManipulatorFunction>(std::flush) || manip == static_cast<ManipulatorFunction>(std::endl))
        {
            std::cout << currentTimeAsString() << " " << _logStream.str() << std::endl;
            _logStream.str("");
        }
        return *this;
    }

private:

    std::stringstream _logStream;

    Logger() {}

    std::string currentTimeAsString()
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char timeStr[std::size("[hh:mm:ss]X")];
        std::strftime(&timeStr[0], sizeof(timeStr), "[%H:%M:%S]", std::localtime(&now));
        return timeStr;
    }
};

} // namespace logger