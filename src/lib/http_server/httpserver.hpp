#pragma once

#include <string>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include "serversocket.hpp"
#include "requesthandler.hpp"
#include "threadpool.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(HTTPServerException, "HTTPServer")

/*
    HTTPServer

    Main server class.
*/
class HTTPServer
{
public:

    HTTPServer(int port, std::string& folder);
    ~HTTPServer();

    // server main loop (blocking)
    void run();

    // set to interrupt main loop
    void cancel();

private:

    // socket to receive connections
    ServerSocket _socket;

    // folder to serve files from
    std::string _folder;

    // main loop interruption flag
    bool _cancelled{false};

    // return values for request handler threads
    std::mutex _requestHandlerFuturesMutex;
    std::condition_variable _requestHandlerFuturesCondVar;
    std::unique_ptr<std::thread> _requestHandlerFuturesCleanupThread;
    std::list<std::future<bool>> _requestHandlerFutures;
    void requestHandlerFuturesCheck();

    // threadpool to service requests
    std::unique_ptr<threadpool::ThreadPool> _threadPool;


};

} // namespace http_server