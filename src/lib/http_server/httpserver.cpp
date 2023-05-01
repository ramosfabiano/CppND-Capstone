#include <filesystem>
#include <stdexcept>
#include "httpserver.hpp"
#include "logger.hpp"

namespace http_server
{

HTTPServer::HTTPServer(int port, std::string& folder): _socket(port), _folder(folder)
{
    if (!std::filesystem::exists(_folder))
    {
        throw HTTPServerException("Folder '" + _folder + "' does not exist!");
    }

    LOGGER() << ">>>>>>> HTTP server started." << std::endl;
    LOGGER() << "Serving files from folder: '" << _folder << std::endl;
}

HTTPServer::~HTTPServer()
{
    LOGGER() << "<<<<<< HTTP server shutting down." << std::endl;
}


void HTTPServer::run()
{
    LOGGER() << "Main loop started." << std::endl;
}

} // namespace http_server