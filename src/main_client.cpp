#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "helper.hpp"
#include "client.hpp"

int main(int argc, char const* argv[]) {
    // usage: client <port> <message>
    cogs::handle_cli_options(argc, argv);

    try {
        boost::program_options::variables_map config;
        cogs::get_configuration(config);
        boost::asio::io_context io_context;

        cogs::Client client{ io_context, config["port"].as<int>() };
        client.send_messages();

        client.wait_answer();
    }
    catch (std::exception& exc) {
        std::cerr << "Exception: " << exc.what() << std::endl;
    }

    return 0;
}
