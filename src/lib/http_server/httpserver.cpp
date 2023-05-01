#include <filesystem>
#include <stdexcept>
#include <signal.h>
#include "httpserver.hpp"
#include "logger.hpp"

namespace http_server
{

// initializing static member
bool HTTPServer::interruptReceived_ = false; // NOLINT

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

void HTTPServer::interruptHandler_(int sig)
{
    HTTPServer::interruptReceived_ = true;
    LOGGER() << "Interrupt received!" << std::endl;
}

void HTTPServer::run()
{
    LOGGER() << "Main loop started." << std::endl;

    // register signal handler (ctrl+c)
    signal(SIGINT, HTTPServer::interruptHandler_);

    // loop until Ctrl+C received
    while(!HTTPServer::interruptReceived_)
    {
        if (_socket.PeekConnection())
        {
            auto requestSocket = _socket.acceptConnection();
            requestSocket->read();
        }
    }

    LOGGER() << "Main loop end." << std::endl;
}

} // namespace http_server