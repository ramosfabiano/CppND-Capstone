#include <algorithm>
#include <string>
#include <filesystem>
#include <sstream>
#include "getmethodhandler.hpp"

namespace http_server
{

GETMethodHandler::GETMethodHandler()
{
}

std::string GETMethodHandler::handleMethod(std::string& resourcePath) const
{
    auto response = HEADMethodHandler::handleMethod(resourcePath);

    //  text/html
    if (hasFileExtension(resourcePath, "html"))
    {
        response += "Content-Type: text/html; charset=utf-8\n";
    }
    //  text/plain
    else if (hasFileExtension(resourcePath, "txt"))
    {
        response += "Content-Type: text/plain; charset=utf-8\n";
    }
    //  text/css
    else if (hasFileExtension(resourcePath, "css"))
    {
        response += "Content-Type: text/css; charset=utf-8\n";
    }
    //  image/jpeg
    else if (hasFileExtension(resourcePath, "jpg") || hasFileExtension(resourcePath, "jpeg"))
    {
        response += "Content-Type: image/jpeg;\n";
    }
    //  image/png
    else if (hasFileExtension(resourcePath, "png"))
    {
        response += "Content-Type: image/png;\n";
    }
    // image/ico
    else if (hasFileExtension(resourcePath, "ico"))
    {
        response += "Content-Type: image/x-icon;\n";
    }
    // unsupported
    else
    {
        throw GETMethodHandlerException("Unsupported resource type");
    }

    response += getResourceContents(resourcePath);

    return std::move(response);
}

bool GETMethodHandler::hasFileExtension(std::string& resourcePath, std::string extension)
{
    std::string lowerCasePath = resourcePath;
    std::transform(lowerCasePath.begin(), lowerCasePath.end(), lowerCasePath.begin(),
                   [](unsigned char chr)
    {
        return std::tolower(chr);
    });

    std::string lowerCaseExt = extension;
    std::transform(lowerCaseExt.begin(), lowerCaseExt.end(), lowerCaseExt.begin(),
                   [](unsigned char chr)
    {
        return std::tolower(chr);
    });

    return lowerCasePath.ends_with(lowerCaseExt);
}

std::string GETMethodHandler::getResourceContents(std::string& resourcePath)
{
    std::string response = "\n";
    std::ifstream inFile(resourcePath);
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    response += buffer.str();
    return std::move(response);
}

} // namespace http_server