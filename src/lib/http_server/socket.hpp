#include <string>
#include "baseexception.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(SocketException, "Socket")

/*
    Socket

    Encapsulates a socket file descriptor.
*/
class Socket
{
public:

    Socket(int port);
    ~Socket();

private:

    // port to bind to
    int _port;

    // native socket file descriptor
    int _socketFileDescriptor;
};

} // namespace http_server