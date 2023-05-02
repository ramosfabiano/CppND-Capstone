#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <array>
#include "logger.hpp"
#include "requestsocket.hpp"

namespace http_server
{

RequestSocket::RequestSocket(int socketFileDescriptor) :
    _socketFileDescriptor(socketFileDescriptor)
{
    // make socket not-blocking
    int optval{1};
    if (ioctl(_socketFileDescriptor, FIONBIO, (char *)&optval) < 0) // NOLINT
    {
        close(_socketFileDescriptor);
        throw RequestSocketException("ioctl() failed.");
    }
    LOGGER() << "Request socket created." << std::endl;
}

RequestSocket::~RequestSocket()
{
    close(_socketFileDescriptor);
    LOGGER() << "Request socket closed." << std::endl;
}

bool RequestSocket::peek(int timeOutSec) const
{
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

// copy constructor
RequestSocket::RequestSocket(const RequestSocket &other) :
    _socketFileDescriptor(other._socketFileDescriptor)
{
    std::cout << "RequestSocket Copy Constructor" << std::endl;
}

// copy assignment operator
RequestSocket &RequestSocket::operator=(const RequestSocket &other)
{
    std::cout << "RequestSocket Copy Assignment Operator" << std::endl;
    close(_socketFileDescriptor);
    _socketFileDescriptor = other._socketFileDescriptor;
    return *this;
}

// move constructor
RequestSocket::RequestSocket(RequestSocket &&other):
    _socketFileDescriptor(other._socketFileDescriptor)
{
    std::cout << "RequestSocket Move Constructor" << std::endl;
    other._socketFileDescriptor = -1;
}

// move assignment operator
RequestSocket &RequestSocket::operator=(RequestSocket &&other)
{
    std::cout << "RequestSocket Move Assignment Operator" << std::endl;
    _socketFileDescriptor = other._socketFileDescriptor;
    other._socketFileDescriptor = -1;
    return *this;
}

std::string RequestSocket::read()
{
    LOGGER() << "Request socket read()." << std::endl;
    std::string request;
    while(peek())
    {
        constexpr int bufferSize{1024};
        std::array<char, bufferSize> buffer{0};
        auto errCode = recv(_socketFileDescriptor, buffer.data(), buffer.size(), 0);
        if (errCode == 0 || (errCode == -1 && errno == EWOULDBLOCK))
        {
            continue;
        }
        if (errCode < 0)
        {
            throw RequestSocketException("Failed to read from socket.");
        }
        request += buffer.data();
    }
    return std::move(request);
}

} // namespace http_server