#include <boost/program_options.hpp>
#include <iostream>
#include <string_view>
#include <logger.hpp>
#include <httpserver.hpp>

constexpr int DEFAULT_PORT = 8080;
constexpr std::string_view DEFAULT_FOLDER = "./site";

int main(int argc, char **argv)
{
    int port{DEFAULT_PORT};
    std::string folder{DEFAULT_FOLDER};

    try
    {
        //
        // parse command line
        //
        boost::program_options::variables_map vm;
        boost::program_options::options_description desc("Options");

        desc.add_options()
        ("port", boost::program_options::value(&port), "Port to listen on.")
        ("folder", boost::program_options::value(&folder), "Folder to serve files from.")
        ("help", "Prints help message.");

        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
        boost::program_options::notify(vm);

        if (vm.count("help"))
        {
            LOGGER() << desc << std::endl;
            return 0;
        }
        if (vm.count("port"))
        {
            port = vm["port"].as<int>();
        }
        if (vm.count("folder"))
        {
            folder = vm["folder"].as<std::string>();
        }

        LOGGER() << "Using port=" << port << " folder='" << folder << "'." << std::endl;

        //
        // start server
        //
        auto http_server = http_server::HTTPServer(port, folder);
        http_server.run();

    }
    catch(http_server::BaseException& e)
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