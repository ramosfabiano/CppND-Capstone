#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <vector>
#include "logger.hpp"
#include "requesthandler.hpp"
#include "methodhandlerfactory.hpp"

namespace http_server
{

RequestHandler::RequestHandler(std::unique_ptr<RequestSocket> requestSocket, std::string& sourceFolder):
    _socket(std::move(requestSocket)),
    _folder(sourceFolder),
    _requestId(getNextRequestId())
{
    //LOGGER() << "Request Handler #" << _requestId <<  " started." << std::endl;
}

RequestHandler::~RequestHandler()
{
    //LOGGER() << "Request Handler #" << _requestId <<  " finished." << std::endl;
}

bool RequestHandler::handleRequest()
{
    LOGGER() << "Handling request #" << _requestId << std::endl;
    try
    {
        _request = _socket->read();
        LOGGER() << "\n" << _request << std::endl;

        // parse request
        std::string method;
        std::string path;
        std::string version;
        std::stringstream requestStream(_request);

        // HTTP method
        requestStream >> method;
        if (method != "GET" && method != "HEAD")
        {
            LOGGER() << "Unsupported method: '" << method << "'" << std::endl;
            return false;
        }

        // resource path
        requestStream >> path;
        if (path == "/")
        {
            path = "/index.html";
        }
        path = _folder + path;

        // check HTTP version
        requestStream >> version;
        if (version != "HTTP/1.1")
        {
            LOGGER() << "Unsupported HTTP version: '" << version << "'" << std::endl;
            return false;
        }

        // instantiate and run the proper method handler though the factory
        auto methodHandler = MethodHandlerFactory::getMethodHandler(method);
        auto response = methodHandler->handleMethod(path);

        // sends the response back to the client
        _socket->write(response);

        return true;
    }
    catch (const std::exception& e)
    {
        LOGGER() << e.what() << std::endl;
        return false;
    }
}

int RequestHandler::getRequestId() const
{
    return _requestId;
}

int RequestHandler::getNextRequestId()
{
    static int nextRequestId_ = 0;
    return ++nextRequestId_ % std::numeric_limits<int>::max();
}

} // namespace http_server
