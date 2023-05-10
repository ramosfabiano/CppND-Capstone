#pragma once

#include <fstream>
#include "headmethodhandler.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(GETMethodHandlerException, "GETMethodHandler")

/*
    GETMethodHandler class.

    HTTP GET method handler.

    Inherits from HEADMethodHandler since the only difference between GET and HEAD is the response body.
*/
class GETMethodHandler: public HEADMethodHandler
{
public:
    GETMethodHandler();
    virtual ~GETMethodHandler() = default;

    std::string handleMethod(std::string& resourceURI) const override;

private:
    static bool hasFileExtension(std::string& resourcePath, std::string extension);
    static std::string getResourceContents(std::string& resourcePath);
};

} // namespace http_server
