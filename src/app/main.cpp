#include <iostream>
#include <string_view>
#include <signal.h>
#include <memory>
#include <boost/program_options.hpp>
#include <logger.hpp>
#include <httpserver.hpp>

constexpr int DEFAULT_PORT = 8080;
constexpr std::string_view DEFAULT_FOLDER = "./site";


// we make it global so it can be accessed by the signal handler
static std::unique_ptr<http_server::HTTPServer> httpServer_; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

/*
    CTRL+C signal handler.
*/
void signalHandler(int sig)
{
    httpServer_->stop();
}

/*
    Main application.

    Parses command line arguments and starts the server.
*/
int main(int argc, char **argv)
{
    int port{DEFAULT_PORT};
    std::string folder{DEFAULT_FOLDER};

    try
    {
        // parse command line
        boost::program_options::variables_map vMap;
        boost::program_options::options_description desc("Options");

        desc.add_options()
        ("port", boost::program_options::value(&port), "Port to listen on.")
        ("folder", boost::program_options::value(&folder), "Folder to serve files from.")
        ("help", "Prints help message.");

        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vMap);
        boost::program_options::notify(vMap);

        if (vMap.count("help") > 0)
        {
            LOGGER() << desc << std::endl;
            return 0;
        }
        if (vMap.count("port") > 0)
        {
            port = vMap["port"].as<int>();
        }
        if (vMap.count("folder") > 0)
        {
            folder = vMap["folder"].as<std::string>();
        }

        LOGGER() << "Using port=" << port << " folder='" << folder << "'." << std::endl;

        // instantiate server
        httpServer_ = std::make_unique<http_server::HTTPServer>(port, folder);

        // register signal handler (ctrl+c)
        signal(SIGINT, signalHandler);

        // start server (blocking)
        httpServer_->run();

    }
    catch(common::BaseException& e)
    {
        LOGGER() << "[ERROR]" << e.what() << std::endl;
        return -1;
    }
    catch (std::exception& e)
    {
        LOGGER() << "[ERROR]" << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        LOGGER() << "Unknown error!" << std::endl;
        return -1;
    }
    return 0;
}