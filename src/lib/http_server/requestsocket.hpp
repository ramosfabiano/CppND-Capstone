#pragma once

#include <string>
#include <memory>
#include "baseexception.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(RequestSocketException, "RequestSocket")

/*
    RequestSocket

    Encapsulates a socket file descriptor, used for handling connections.
*/
class RequestSocket
{
public:
    RequestSocket(int socketFileDescriptor);
    ~RequestSocket();

    std::string read();

private:

    // native socket file descriptor
    int _socketFileDescriptor;

    // checks if data is available at the socket (non-blocking)
    bool peekForData(int timeOutSec = 1);

};

typedef std::unique_ptr<RequestSocket> RequestSocketPtr;

} // namespace http_server