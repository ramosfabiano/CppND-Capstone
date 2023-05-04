#pragma once

#include <string>
#include <list>
#include <memory>
#include <thread>
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

    // threadpool to service requests
    std::unique_ptr<threadpool::ThreadPool> _threadPool;

    // main loop interruption flag
    bool _cancelled{false};

};

} // namespace http_server