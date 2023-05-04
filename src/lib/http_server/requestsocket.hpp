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

    // copy semantics
    RequestSocket(const RequestSocket &other) = delete;
    RequestSocket &operator=(const RequestSocket &other) = delete;

    // move semantics
    RequestSocket(RequestSocket &&other);
    RequestSocket& operator=(RequestSocket &&other);

    std::string read();

private:

    // native socket file descriptor
    int _socketFileDescriptor;

    // checks if data is available at the socket (non-blocking)
    bool peek(int timeOutSec = 1) const;

};

} // namespace http_server