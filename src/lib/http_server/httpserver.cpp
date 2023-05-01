#include <filesystem>
#include <stdexcept>
#include "httpserver.hpp"
#include "logger.hpp"
#include "requesthandler.hpp"

namespace http_server
{

HTTPServer::HTTPServer(int port, std::string& folder): _socket(port), _folder(folder), _cancelled(false)
{
    if (!std::filesystem::exists(_folder))
    {
        throw HTTPServerException("Folder '" + _folder + "' does not exist!");
    }

    LOGGER() << ">>>>>>> HTTP server started." << std::endl;
    LOGGER() << "Serving files from folder: '" << _folder << std::endl;
}

HTTPServer::~HTTPServer()
{
    LOGGER() << "<<<<<< HTTP server shutting down." << std::endl;
}

void HTTPServer::run()
{
    LOGGER() << "Main loop started." << std::endl;

    // loop until cancelled
    while(!_cancelled)
    {
        if (_socket.peekConnection())
        {
            auto requestSocket = _socket.acceptConnection();

            // TODO: add to collection, start new thead / task
            auto requestHandler = std::make_unique<RequestHandler>(std::move(requestSocket));
            requestHandler->start();
        }
    }

    LOGGER() << "Main loop end." << std::endl;
}

void HTTPServer::cancel()
{
    LOGGER() << "Main loop interruption requested." << std::endl;
    _cancelled = true;
}

} // namespace http_server