#pragma once

#include <string>
#include <memory>
#include <baseexception.hpp>
#include "getmethodhandler.hpp"
#include "headmethodhandler.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(MethodHandlerFactoryException, "MethodHandlerFactory")

/*
    MethodHandlerFactory

    Instantiates the proper method handler for the request.
*/
class MethodHandlerFactory
{
public:

    // only GET and HEAD methods are supported for basic operation.
    static std::unique_ptr<MethodHandler> getMethodHandler(std::string& method)
    {
        if (method == "GET")
        {
            return std::make_unique<GETMethodHandler>();
        }
        else if (method == "HEAD")
        {
            return std::make_unique<HEADMethodHandler>();
        }
        else
        {
            throw MethodHandlerFactoryException("Method handler not implemented.");
        }
    }
};

} // namespace http_server
