#pragma once

#include <string>
#include <memory>
#include <baseexception.hpp>
#include "getmethodhandler.hpp"
#include "headmethodhandler.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(MethodHandlerFactoryException, "MethodHandlerFactory")

class MethodHandlerFactory
{
public:

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
            throw MethodHandlerFactoryException("Method not implemented");
        }
    }
};

} // namespace http_server
