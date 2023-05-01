#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <array>
#include "logger.hpp"
#include "requestsocket.hpp"

namespace http_server
{

RequestSocket::RequestSocket(int socketFileDescriptor) :
    _socketFileDescriptor(socketFileDescriptor)
{
    LOGGER() << "Request socket created." << std::endl;
}

RequestSocket::~RequestSocket()
{
    LOGGER() << "Request socket closed." << std::endl;
}

void RequestSocket::read()
{
    LOGGER() << "Request socket read()." << std::endl;

    constexpr int bufferSize{4098};
    std::array<char, bufferSize> buffer{0};
    ::read(_socketFileDescriptor, buffer.data(), bufferSize);

    LOGGER() << buffer.data() << std::endl;
}

void RequestSocket::write()
{
    LOGGER() << "Request socket write()." << std::endl;
}


} // namespace http_server