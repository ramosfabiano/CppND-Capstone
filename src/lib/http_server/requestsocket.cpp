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
    //LOGGER() << "Request socket created." << std::endl;
}

RequestSocket::~RequestSocket()
{
    if (_socketFileDescriptor > 0)
    {
        shutdown(_socketFileDescriptor, SHUT_RDWR);
        close(_socketFileDescriptor);
    }
    //LOGGER() << "Request socket closed." << std::endl;
}

// move constructor
RequestSocket::RequestSocket(RequestSocket &&other):
    _socketFileDescriptor(other._socketFileDescriptor)
{
    other._socketFileDescriptor = -1;
}

// move assignment operator
RequestSocket &RequestSocket::operator=(RequestSocket &&other)
{
    _socketFileDescriptor = other._socketFileDescriptor;
    other._socketFileDescriptor = -1;
    return *this;
}

bool RequestSocket::peek(int timeOutSec) const
{
    if (_socketFileDescriptor <= 0)
    {
        throw RequestSocketException("Socket is closed.");
    }

    // NOLINTBEGIN    (disabling clang-tidy warnings for the C-style block below)
    fd_set set;
    struct timeval timeout
    {
        0
    };
    FD_ZERO(&set);                       // NOLINT
    FD_SET(_socketFileDescriptor, &set); // NOLINT
    timeout.tv_sec = timeOutSec;
    timeout.tv_usec = 0;
    // rc < 0: error
    // rc = 0 : timeout
    // rc > 0: successs
    int rc = select(_socketFileDescriptor + 1, &set, NULL, NULL, &timeout);
    // NOLINTEND

    return rc > 0 && FD_ISSET(_socketFileDescriptor, &set); // NOLINT
}

std::string RequestSocket::read()
{
    if (_socketFileDescriptor <= 0)
    {
        throw RequestSocketException("Socket is closed.");
    }

    //LOGGER() << "Request socket read()." << std::endl;
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

void RequestSocket::write(const std::string& data) const
{
    if (_socketFileDescriptor <= 0)
    {
        throw RequestSocketException("Socket is closed.");
    }

    //LOGGER() << "Request socket write()." << std::endl;
    long dataLen = (long)data.length();
    long totalBytesSent = 0;

    while (totalBytesSent < dataLen)
    {
        long bytesSent = send(_socketFileDescriptor, &data[totalBytesSent], dataLen - totalBytesSent, 0);
        if (bytesSent == -1)
        {
            throw RequestSocketException("Failed to write to socket.");
        }
        totalBytesSent += bytesSent;
    }
}

} // namespace http_server
