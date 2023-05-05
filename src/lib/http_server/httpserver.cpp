#include <filesystem>
#include <stdexcept>
#include "httpserver.hpp"
#include "logger.hpp"

namespace http_server
{

HTTPServer::HTTPServer(int port, std::string& folder):
    _socket(port),
    _folder(folder),
    _stopRequested(false),
    // handling request tasks is an I/O bounded task, so the threadpool could be created with more logical threads than number of hardware threads
    _threadPool(std::make_unique<threadpool::ThreadPool>(std::thread::hardware_concurrency() * 4))
{
    if (!std::filesystem::exists(_folder))
    {
        throw HTTPServerException("Folder '" + _folder + "' does not exist!");
    }
    LOGGER() << "HTTP server started." << std::endl;
    LOGGER() << "Serving files from folder: '" << _folder << std::endl;
}

HTTPServer::~HTTPServer()
{
    LOGGER() << "HTTP server is shutting down..." << std::endl;

    long failedRequests = 0;
    long sucessfulRequests = 0;
    for(auto& future : _requestHandlerFutures)
    {
        auto result = future.get();
        if (result)
        {
            ++sucessfulRequests;
        }
        else
        {
            ++failedRequests;
        }
    }
    _requestHandlerFutures.clear();

    LOGGER() << "Total requests: " << (sucessfulRequests + failedRequests) << std::endl;
    LOGGER() << "Sucessful requests: " << sucessfulRequests << std::endl;
    LOGGER() << "Failed requests: " << failedRequests << std::endl;

    _threadPool.reset();
    
    LOGGER() << "HTTP server stopped." << std::endl;
}

void HTTPServer::run()
{
    LOGGER() << "HTTPServer main loop started." << std::endl;
    // loop until cancelled
    while(!_stopRequested)
    {
        if (_socket.peekConnection())
        {
            // accepts the connection
            auto requestSocket = _socket.acceptConnection();

            // create request handler
            auto requestHandler = std::make_shared<RequestHandler>(std::move(requestSocket));

            // add task to threadpool and store future
            _requestHandlerFutures.emplace_back(_threadPool->addTask(std::bind<bool>(&RequestHandler::start, requestHandler)));
        }
    }
    LOGGER() << "HTTPServer main loop end." << std::endl;
}

void HTTPServer::stop()
{
    LOGGER() << "HTTPServer stop requested." << std::endl;
    _stopRequested = true;
}

} // namespace http_server