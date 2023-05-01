#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "logger.hpp"
#include "serversocket.hpp"

namespace http_server
{

ServerSocket::ServerSocket(int port) :
    _port(port),
    // creates socket
    _socketFileDescriptor(socket(AF_INET, SOCK_STREAM, 0))
{
    if (_socketFileDescriptor < 0)
    {
        throw ServerSocketException("Socket creation failed.");
    }
    LOGGER() << "Server socket opened." << std::endl;

    // bind to port
    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    int bindRc = bind(_socketFileDescriptor, (struct sockaddr*)(&address), sizeof(address));
    if (bindRc < 0)
    {
        std::string errMsg = "Failed binding to socket to port " + std::to_string(_port) + ". (rc=" + std::to_string(bindRc) + ").";
        throw ServerSocketException(errMsg);
    }
    // NOLINTEND

    // listen for connections
    if (listen(_socketFileDescriptor, _maxPendingConnections) < 0)
    {
        throw ServerSocketException("Failed to listen on socket.");
    }

    LOGGER() << "Listening on port " << _port << std::endl;
}

ServerSocket::~ServerSocket()
{
    shutdown(_socketFileDescriptor, SHUT_RDWR);
    close(_socketFileDescriptor);
    LOGGER() << "Server socket closed." << std::endl;
}

bool ServerSocket::PeekConnection(int timeOutSec)
{
    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(_socketFileDescriptor, &set);
    timeout.tv_sec = timeOutSec;
    timeout.tv_usec = 0;
    // NOLINTEND
    return (select(_socketFileDescriptor + 1, &set, NULL, NULL, &timeout) > 0);
}

RequestSocketPtr ServerSocket::acceptConnection()
{
    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    int _newSocket = accept(_socketFileDescriptor, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (_newSocket < 0)
    {
        throw ServerSocketException("Failed to accept connection.");
    }
    // NOLINTEND
    return std::make_unique<RequestSocket>(_newSocket);
}


} // namespace http_server