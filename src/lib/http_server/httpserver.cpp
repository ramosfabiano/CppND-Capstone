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
    LOGGER() << "Serving files from folder: '" << _folder << "'" << std::endl;
}

HTTPServer::~HTTPServer()
{
    LOGGER() << "<<<<<< HTTP server shutting down." << std::endl;
    cancel();
    {
        std::lock_guard <std::mutex> lock(_requestHandlerFuturesMutex);
        _requestHandlerFutures.clear();
    }
    _requestHandlerFuturesCondVar.notify_all();
    if (_requestHandlerFuturesCleanupThread)
    {
        _requestHandlerFuturesCleanupThread->join();
    }
    _threadPool.reset();
}

void HTTPServer::run()
{
    LOGGER() << "Main loop started." << std::endl;

    // start handler thread
    _requestHandlerFuturesCleanupThread = std::make_unique<std::thread>(&HTTPServer::requestHandlerFuturesCheck, this);

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
            {
                std::lock_guard<std::mutex> lock(_requestHandlerFuturesMutex);
                _requestHandlerFutures.emplace_back(_threadPool->addTask(std::bind<bool>(&RequestHandler::start, requestHandler)));
            }
            _requestHandlerFuturesCondVar.notify_all();
        }
    }
    LOGGER() << "Main loop end." << std::endl;
}

void HTTPServer::cancel()
{
    LOGGER() << "Main loop interruption requested." << std::endl;
    _cancelled = true;
}

void HTTPServer::requestHandlerFuturesCheck()
{
    LOGGER() << "RequestHandler futures thread started." << std::endl;
    while(true)
    {
        {
            std::unique_lock<std::mutex> lock(_requestHandlerFuturesMutex);
            _requestHandlerFuturesCondVar.wait(lock, [this] ()
            {
                return !_requestHandlerFutures.empty() || _cancelled;
            });
            if (_cancelled)
            {
                LOGGER() << "RequestHandler futures thread end." << std::endl;
                return;
            }
            if (_requestHandlerFutures.empty())
            {
                continue;
            }
            for (auto it = _requestHandlerFutures.begin(); it != _requestHandlerFutures.end(); ++it)
            {
                if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                {
                    if (it->get())
                    {
                        // TODO: which one?
                        LOGGER() << "RequestHandler suceeed." << std::endl;
                    }
                    else
                    {
                        LOGGER() << "RequestHandler failed." << std::endl;
                    }
                    it = _requestHandlerFutures.erase(it);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} // namespace http_server