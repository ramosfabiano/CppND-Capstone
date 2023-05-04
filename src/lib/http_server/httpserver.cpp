#include <filesystem>
#include <stdexcept>
#include "httpserver.hpp"
#include "logger.hpp"

namespace http_server
{

HTTPServer::HTTPServer(int port, std::string& folder):
    _socket(port),
    _folder(folder),
    _cancelled(false),
    // handling request tasks is an I/O bounded task, so the threadpool could be created with more logical threads than number of hardware threads
    _threadPool(std::make_unique<threadpool::ThreadPool>(std::thread::hardware_concurrency()))
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
    _threadPool.reset();
    _requestHandlerFutures.clear();
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
            auto requestHandler = std::make_shared<RequestHandler>(std::move(requestSocket));

            // add task to threadpool (and store future)
            _requestHandlerFutures.emplace_back(_threadPool->addTask(std::bind<bool>(&RequestHandler::start, requestHandler)));
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