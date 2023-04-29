#include <string>

/*
    HTTPServer

    Main server class.
*/
namespace http_server
{

class HTTPServer
{
public:

    HTTPServer(int port, std::string& folder);
    ~HTTPServer();

    // server main loop (blocking)
    void run();

private:

    // port to listen on
    int _port;

    // folder to serve files from
    std::string _folder;
};

} // namespace http_server