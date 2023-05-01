#include <filesystem>
#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>
#include "logger.hpp"
#include "socket.hpp"

namespace http_server
{

Socket::Socket(int port) :
    _port(port),
    // creates socket
    _socketFileDescriptor(socket(AF_INET, SOCK_STREAM, 0))
{
    if (_socketFileDescriptor < 0)
    {
        throw SocketException("Socket creation failed.");
    }
    LOGGER() << "Socket opened." << std::endl;

    // bind to port
    struct sockaddr_in address
    {
        AF_INET, INADDR_ANY, htons(_port)
    };
    // NOLINTNEXTLINE   (disabling clang-tidy warning "cppcoreguidelines-pro-type-cstyle-cast" for the line below)
    if (bind(_socketFileDescriptor, (struct sockaddr*)(&address), sizeof(address))  < 0)
    {
        throw SocketException("Failed binding to socket to port.");
    }
    LOGGER() << "Listening on port " << _port << std::endl;
}

Socket::~Socket()
{
    shutdown(_socketFileDescriptor, SHUT_RDWR);
    LOGGER() << "Socket closed." << std::endl;
}

} // namespace http_server