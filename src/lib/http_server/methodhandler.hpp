#pragma once

#include <string>
#include <memory>
#include <baseexception.hpp>
#include <logger.hpp>
namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(MethodHandlerException, "MethodHandler")

class MethodHandler
{
public:
    virtual ~MethodHandler() = default;

    virtual std::string handleMethod(std::string& resourceURI) const = 0;

protected:

    MethodHandler() = default;

    std::string currentDateTimeAsString() const
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::array<char, std::size("%a, %d %b %Y %HH:%HM:%HS ")> timeStr{0};
        std::strftime(timeStr.data(), timeStr.size(), "%a, %d %b %Y %H:%M:%S", std::localtime(&now));
        return timeStr.data();
    }

    // [/path][?query][#fragment]
    std::string extractPathFromURI(std::string& uri) const
    {
        std::string pathComponent = uri.substr(0, uri.find_first_of('?'));
        return pathComponent;
    }
};

} // namespace http_server
