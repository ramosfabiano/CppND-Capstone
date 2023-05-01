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

    void read();
    void write();

private:

    // native socket file descriptor
    int _socketFileDescriptor;

};

typedef std::unique_ptr<RequestSocket> RequestSocketPtr;

} // namespace http_server