#include <filesystem>
#include <stdexcept>
#include "logger.hpp"
#include "requesthandler.hpp"

namespace http_server
{

RequestHandler::RequestHandler(std::unique_ptr<RequestSocket> requestSocket): _socket(std::move(requestSocket))
{
    LOGGER() << ">>>>>>> Request Handler started." << std::endl;
}

RequestHandler::~RequestHandler()
{
    LOGGER() << "<<<<<< Request Handler shutting down." << std::endl;
}

bool RequestHandler::start()
{
    try
    {
        _request = _socket->read();
        std::cout << _request << std::endl;
        return true;

    }
    catch (const std::exception& e)
    {
        LOGGER() << e.what() << std::endl;
        return false;
    }
}

} // namespace http_server
