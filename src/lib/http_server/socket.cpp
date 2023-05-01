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
    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);      
    int bindRc = bind(_socketFileDescriptor, (struct sockaddr*)(&address), sizeof(address));
    if (bindRc < 0)
    {
        std::string errMsg = "Failed binding to socket to port " + std::to_string(_port) + ". (rc=" + std::to_string(bindRc) + ").";
        throw SocketException(errMsg);
    }
    // NOLINTEND
    LOGGER() << "Listening on port " << _port << std::endl;
}

Socket::~Socket()
{
    shutdown(_socketFileDescriptor, SHUT_RDWR);
    LOGGER() << "Socket closed." << std::endl;
}

} // namespace http_server