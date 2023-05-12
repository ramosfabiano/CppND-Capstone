#pragma once

#include <chrono>
#include <ctime>
#include <string>
#include "methodhandler.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(HeadMethodHandlerException, "HEADMethodHandler")

/*
    HEADMethodHandler class.

    HTTP HEAD method handler.
*/
class HEADMethodHandler: public MethodHandler
{
public:
    HEADMethodHandler();
    virtual ~HEADMethodHandler() = default;

    virtual std::string handleMethod(std::string& resourceURI) const override;
};

} // namespace http_server
