#include <iostream>
#include <fstream>
#include <map>
#include <variant>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "helper.hpp"
#include "server.hpp"

int main(int argc, char const* argv[]) {
    // usage: client <port> <message>
    cogs::handle_cli_options(argc, argv);

    try {
        boost::program_options::variables_map config;
        cogs::get_configuration(config);
        boost::asio::io_context io_context;
        
        cogs::Server server{
            io_context,
            config["port"].as<int>(),
            config["connection_timeout"].as<int>(),
            config["max_file_size"].as<int>(),
            config["file_prefix"].as<std::string>()
        };

        io_context.run();
    } catch (std::exception& exc) {
        std::cerr << "Exception: " << exc.what() << std::endl;
    }

    return 0;
}
