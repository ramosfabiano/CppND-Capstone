#pragma once

#include <string>
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

    bool PeekConnection(int timeOutSec = 5);
    RequestSocketPtr acceptConnection();

private:

    // port to bind to
    int _port;

    // native socket file descriptor
    int _socketFileDescriptor;

    // max pending connections befgore refusing
    const int _maxPendingConnections{10};
};

} // namespace http_server