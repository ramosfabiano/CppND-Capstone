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

    RequestHandler(RequestSocketPtr&& requestSocket);
    ~RequestHandler();

    void start();

private:

    RequestSocketPtr _socket;

};

} // namespace http_server