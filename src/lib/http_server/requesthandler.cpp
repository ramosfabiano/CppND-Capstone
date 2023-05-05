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

bool RequestHandler::start()
{
    LOGGER() << "Handling request #" << _requestId << std::endl;
    try
    {
        _request = _socket->read();
        //std::cout << _request << std::endl;
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
