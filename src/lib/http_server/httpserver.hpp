#include <string>
#include "socket.hpp"

namespace http_server
{

IMPLEMENT_CUSTOM_EXCEPTION(HTTPServerException, "HTTPServer")

/*
    HTTPServer

    Main server class.
*/
class HTTPServer
{
public:

    HTTPServer(int port, std::string& folder);
    ~HTTPServer();

    // server main loop (blocking)
    void run();

private:

    // socket to receive connections
    Socket _socket;

    // folder to serve files from
    std::string _folder;
};

} // namespace http_server