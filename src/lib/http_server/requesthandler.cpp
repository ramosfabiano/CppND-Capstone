#include <filesystem>
#include <stdexcept>
#include <limits>
#include "logger.hpp"
#include "requesthandler.hpp"

namespace http_server
{

RequestHandler::RequestHandler(std::unique_ptr<RequestSocket> requestSocket):
    _socket(std::move(requestSocket)),
    _requestId(getNextRequestId())
{
    //LOGGER() << "Request Handler #" << _requestId <<  " started." << std::endl;
}

RequestHandler::~RequestHandler()
{
    //LOGGER() << "Request Handler #" << _requestId <<  " finished." << std::endl;
}

bool RequestHandler::handleRequest()
{
    LOGGER() << "Handling request #" << _requestId << std::endl;
    try
    {
        _request = _socket->read();
        //std::cout << _request << std::endl;

        std::string response;
        response += "HTTP/1.1 404 Not Found\n";
        response += "Date: Sun, 05 May 2023 10:36:20 GMT\n";
        response += "Server: HTTServer Capstone\n";
        response += "Content-Length: 230\n";
        response += "Connection: Closed\n";
        response += "Content-Type: text/html; charset=iso-8859-1\n";

        return true;
    }
    catch (const std::exception& e)
    {
        LOGGER() << e.what() << std::endl;
        return false;
    }
}

int RequestHandler::getRequestId() const
{
    return _requestId;
}

int RequestHandler::getNextRequestId()
{
    static int nextRequestId_ = 0;
    return ++nextRequestId_ % std::numeric_limits<int>::max();
}

} // namespace http_server
