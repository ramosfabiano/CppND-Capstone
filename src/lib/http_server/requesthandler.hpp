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
    int getRequestId() const;

private:

    std::unique_ptr<RequestSocket> _socket;

    std::string _request;

    int _requestId;

    static int getNextRequestId();
};

} // namespace http_server
