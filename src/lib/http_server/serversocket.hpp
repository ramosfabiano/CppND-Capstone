#pragma once

#include <string>
#include <memory>
#include "baseexception.hpp"
#include "requestsocket.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(ServerSocketException, "ServerSocket")

/*
    ServerSocket

    Encapsulates a socket file descriptor, used for listening to connections.
*/
class ServerSocket
{
public:

    ServerSocket(int port);
    ~ServerSocket();

    // copy semantics
    ServerSocket(const ServerSocket &other) = delete;
    ServerSocket &operator=(const ServerSocket &other) = delete;

    // move semantics
    ServerSocket(ServerSocket &&other);
    ServerSocket& operator=(ServerSocket &&other);

    // checks if a client connection is pending
    bool peekConnection(int timeOutSec = 5) const;

    // accepts a client connection
    std::unique_ptr<RequestSocket> acceptConnection() const;

private:

    // port to bind to
    int _port;

    // native socket file descriptor
    int _socketFileDescriptor;

    // max pending connections before start refusing
    const int _maxPendingConnections{10};
};

} // namespace http_server