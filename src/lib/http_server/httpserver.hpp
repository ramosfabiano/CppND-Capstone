#include <string>

namespace http_server
{

class HTTPServer
{
public:
    HTTPServer(int port, std::string& folder);
    ~HTTPServer();
    void run();
private:
    int _port;
    std::string _folder;
};

}