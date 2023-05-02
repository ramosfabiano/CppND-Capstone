#include <filesystem>
#include <stdexcept>
#include "logger.hpp"
#include "requesthandler.hpp"

namespace http_server
{

RequestHandler::RequestHandler(std::unique_ptr<RequestSocket>&& requestSocket): _socket(std::move(requestSocket))
{
    LOGGER() << ">>>>>>> Request Handler started." << std::endl;
}

RequestHandler::~RequestHandler()
{
    LOGGER() << "<<<<<< Request Handler shutting down." << std::endl;
}

void RequestHandler::start()
{
    auto request = _socket->read();
    std::cout << request << std::endl;
}

} // namespace http_server