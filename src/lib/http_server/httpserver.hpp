#pragma once

#include <string>
#include "serversocket.hpp"

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

private:
    // Ctrl+C signal handler
    // cancellation will apply to all running instances of HTTPServer
    static bool interruptReceived_;
    static void interruptHandler_(int);

    // socket to receive connections
    ServerSocket _socket;

    // folder to serve files from
    std::string _folder;
};

} // namespace http_server