#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <filesystem>
#include "logger.hpp"
#include "headmethodhandler.hpp"

namespace http_server
{

HEADMethodHandler::HEADMethodHandler()
{
}

std::string HEADMethodHandler::handleMethod(std::string& resourcePath) const
{
    std::string response;

    try
    {
        std::filesystem::path filePath{resourcePath};

        response += "HTTP/1.1 200 OK\n";
        response += std::string("Date: ") + currentDateTimeAsString() + std::string(" GMT\n");
        response += "Server: Simple HTTP Server Capstone\n";
        response += "Content-Length: " + std::to_string(std::filesystem::file_size(filePath)) + "\n";
        response += "Connection: Closed\n";
    }
    catch(...)
    {
        response += "HTTP/1.1 404 Not Found\n";
        response += std::string("Date: ") + currentDateTimeAsString() + std::string(" GMT\n");
        response += "Server:  Simple HTTP Server Capstone\n";
        response += "Content-Length: 0\n";
        response += "Connection: Closed\n";
    }

    LOGGER() << "\n" << response << std::endl;

    return std::move(response);
}

} // namespace http_server