#pragma once

#include <string>
#include "requestsocket.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(RequestHandlerException, "RequestHandler")

/*
    RequestHandler

    Handles the HTTP request.

    This class is responsible for receiving the request from the client, invoking the proper method handler, and send respnse back to the client.
*/
class RequestHandler
{
public:

    RequestHandler(std::unique_ptr<RequestSocket> requestSocket, std::string& sourceFolder);
    ~RequestHandler();

    // handles the request
    bool handleRequest();

    // returns the request id
    int getRequestId() const;

private:

    std::unique_ptr<RequestSocket> _socket;
    std::string _request;
    std::string _folder;
    int _requestId;

    // generates a sequential request id
    static int getNextRequestId();
};

} // namespace http_server
