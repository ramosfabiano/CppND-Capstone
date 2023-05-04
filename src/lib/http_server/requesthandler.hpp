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

    RequestHandler(std::unique_ptr<RequestSocket> requestSocket);
    ~RequestHandler();

    bool start();

private:

    std::unique_ptr<RequestSocket> _socket;

    std::string _request;

};

} // namespace http_server
