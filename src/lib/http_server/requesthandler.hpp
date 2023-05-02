#pragma once

#include <string>
#include "requestsocket.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(RequestHandlerException, "RequestHandler")

/*
    RequestHandler

    Handles the HTTP request.
*/
class RequestHandler
{
public:

    RequestHandler(std::unique_ptr<RequestSocket>&& requestSocket);
    ~RequestHandler();

    void start();

private:

    std::unique_ptr<RequestSocket> _socket;

};

} // namespace http_server