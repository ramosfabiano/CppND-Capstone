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
    std::string request{""};

    while(true)
    {
        constexpr int bufferSize{1024};
        std::array<char, bufferSize> buffer{0};
        //LOGGER() << "calling recv..." << std::endl;
        auto rc = ::recv(_socketFileDescriptor, buffer.data(), buffer.size(), 0);
        //LOGGER() << "recv rc=" << rc <<  " errno: " << errno << std::endl;
        if (rc == 0 || (rc == -1 && errno == EWOULDBLOCK))
        {
            break;
        }
        else
        {
            if (rc < 0)
            {
                throw RequestSocketException("Failed to read from socket.");
            }
            else
            {
                request += buffer.data();
            }
        }
    }

    LOGGER() << request << std::endl;
}

void RequestSocket::write()
{
    LOGGER() << "Request socket write()." << std::endl;
}


} // namespace http_server