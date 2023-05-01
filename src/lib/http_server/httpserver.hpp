#pragma once

#include <string>
#include <list>
#include <memory>
#include <thread>
#include "serversocket.hpp"
#include "requesthandler.hpp"

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

    // collection of request handlers
    std::list<std::unique_ptr<RequestHandler>> _requestHandlers;

    // collection of request handler threads
    std::list<std::thread> _threads;

    // main loop interruption flag
    bool _cancelled{false};

};

} // namespace http_server