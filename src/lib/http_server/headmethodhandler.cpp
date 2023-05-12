#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include "logger.hpp"
#include "headmethodhandler.hpp"

namespace http_server
{

HEADMethodHandler::HEADMethodHandler()
{
}

std::string HEADMethodHandler::handleMethod(std::string& resourceURI) const
{
    // disregards any query string or fragments
    auto resourcePath = extractPathFromURI(resourceURI);

    std::string response;

    try
    {
        response += "HTTP/1.1 200 OK\n";
        response += std::string("Date: ") + currentDateTimeAsString() + std::string(" GMT\n");
        response += "Server: Simple HTTP Server Capstone\n";
        response += "Content-Length: " + std::to_string(boost::filesystem::file_size(resourcePath)) + "\n";
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