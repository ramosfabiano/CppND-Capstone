#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cassert>
#include "logger.hpp"
#include "serversocket.hpp"

namespace http_server
{

ServerSocket::ServerSocket(int port) :
    _port(port),
    _socketFileDescriptor(socket(AF_INET, SOCK_STREAM, 0))
{
    int optval{1};

    if (_socketFileDescriptor < 0)
    {
        throw ServerSocketException("Socket creation failed.");
    }
    LOGGER() << "Server socket opened." << std::endl;

    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)

    // make socket reusable
    if (setsockopt(_socketFileDescriptor, SOL_SOCKET,  SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0)
    {
        close(_socketFileDescriptor);
        throw ServerSocketException("setsockopt() failed.");
    }

    // make socket not-blocking
    if (ioctl(_socketFileDescriptor, FIONBIO, (char *)&optval) < 0)
    {
        close(_socketFileDescriptor);
        throw ServerSocketException("ioctl() failed.");
    }

    // bind to port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    if (bind(_socketFileDescriptor, (struct sockaddr*)(&address), sizeof(address)) < 0)
    {
        close(_socketFileDescriptor);
        std::string errMsg = "Failed binding to socket to port " + std::to_string(_port);
        throw ServerSocketException(errMsg);
    }

    // listen for connections
    if (listen(_socketFileDescriptor, _maxPendingConnections) < 0)
    {
        close(_socketFileDescriptor);
        throw ServerSocketException("Failed to listen on socket.");
    }

    // NOLINTEND

    LOGGER() << "Listening on port " << _port << std::endl;
}

ServerSocket::~ServerSocket()
{
    if (_socketFileDescriptor > 0)
    {
        shutdown(_socketFileDescriptor, SHUT_RDWR);
        close(_socketFileDescriptor);
    }
    LOGGER() << "Server socket closed." << std::endl;
}

// move constructor
ServerSocket::ServerSocket(ServerSocket &&other):
    _port(other._port),
    _socketFileDescriptor(other._socketFileDescriptor)
{
    other._socketFileDescriptor = -1;
}

// move assignment operator
ServerSocket &ServerSocket::operator=(ServerSocket &&other)
{
    _port = other._port;
    _socketFileDescriptor = other._socketFileDescriptor;
    other._port = -1;
    other._socketFileDescriptor = -1;
    return *this;
}

bool ServerSocket::peekConnection(int timeOutSec) const
{
    if (_socketFileDescriptor <= 0)
    {
        throw ServerSocketException("Socket is closed.");
    }

    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(_socketFileDescriptor, &set);
    timeout.tv_sec = timeOutSec;
    timeout.tv_usec = 0;
    // rc < 0: error
    // rc = 0 : timeout
    // rc > 0: successs
    int rc = select(_socketFileDescriptor + 1, &set, NULL, NULL, &timeout);
    // NOLINTEND

    return rc > 0 && FD_ISSET(_socketFileDescriptor, &set);
}

std::unique_ptr<RequestSocket> ServerSocket::acceptConnection() const
{
    if (_socketFileDescriptor <= 0)
    {
        throw ServerSocketException("Socket is closed.");
    }

    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    int rc = accept(_socketFileDescriptor, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (rc < 0)
    {
        assert(errno != EWOULDBLOCK); // should not happen, as we peeked before with select()
        throw ServerSocketException("Failed to accept connection.");
    }
    // NOLINTEND

    // rc is the new socket file descriptor
    return std::make_unique<RequestSocket>(rc);
}

} // namespace http_server