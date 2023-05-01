#include <filesystem>
#include <stdexcept>
#include "httpserver.hpp"
#include "logger.hpp"

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
    for (auto& thread : _threads)
    {
        thread.join();
    }
    _threads.clear();
    _requestHandlers.clear();
}

void HTTPServer::run()
{
    LOGGER() << "Main loop started." << std::endl;

    // loop until cancelled
    while(!_cancelled)
    {
        if (_socket.peekConnection())
        {
            // accepts the connection
            auto requestSocket = _socket.acceptConnection();

            // create request handler
            auto& requestHandler = _requestHandlers.emplace_back(std::make_unique<RequestHandler>(std::move(requestSocket)));

            // handles request in a separate thread
            auto& handlerThread = _threads.emplace_back(&RequestHandler::start, requestHandler.get());
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