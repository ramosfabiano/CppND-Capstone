#include <filesystem>
#include <stdexcept>
#include "httpserver.hpp"
#include "logger.hpp"

namespace http_server
{

HTTPServer::HTTPServer(int port, std::string& folder)
{
    if (!std::filesystem::exists(folder))
    {
        throw std::runtime_error("Folder '" + folder + "' does not exist!");
    }
    _port = port;
    _folder = folder;
    LOGGER() << ">>>>>>> HTTP server started." << std::endl;
}

HTTPServer::~HTTPServer()
{
    LOGGER() << "<<<<<< HTTP server shutting down." << std::endl;
}


void HTTPServer::run()
{
    LOGGER() << "Listening on port " << _port << std::endl;
    LOGGER() << "Serving files from folder: '" << _folder << std::endl;
}

} // namespace http_server