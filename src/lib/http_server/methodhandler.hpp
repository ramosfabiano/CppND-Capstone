#pragma once

#include <string>
#include <memory>
#include <baseexception.hpp>

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(MethodHandlerException, "MethodHandler")

class MethodHandler
{
public:
    virtual ~MethodHandler() = default;

    virtual std::string handleMethod(std::string& resourcePath) const = 0;

protected:

    MethodHandler() = default;

    std::string currentDateTimeAsString() const
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::array<char, std::size("%a, %d %b %Y %HH:%HM:%HS ")> timeStr{0};
        std::strftime(timeStr.data(), timeStr.size(), "%a, %d %b %Y %H:%M:%S", std::localtime(&now));
        return timeStr.data();
    }
};

} // namespace http_server
