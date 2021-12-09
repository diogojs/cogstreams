#include <iostream>
#include <fstream>
#include <string>

#include "helper.hpp"

namespace cogs {
using namespace boost::program_options;

void handle_cli_options(int argc, char const *argv[], std::shared_ptr<variables_map> vm) {
    try {
        options_description description("Allowed options");
        description.add_options()
            ("help,h", "Show this help message")
            ("host", value<std::string>()->default_value("localhost"), "Hostname to connect to")
            ("port", value<int>()->default_value(8042), "Port to connect")
            ("connection_timeout", value<int>()->default_value(5), "Timeout before closing a connection without data")
            ("max_file_size", value<int>()->default_value(8), "Maximum filesize in bytes")
            ("file_prefix", value<std::string>()->default_value("prefixo_"), "Filename prefix");

        store(parse_command_line(argc, argv, description), (*vm));

        if (vm->count("help")) {
            std::cout << "*** Usage ***\n"
                << "Client: cogclient [host] [port]\n"
                << "Server: cogserver [port] [connection_timeout] [max_file_size] [file_prefix]"
                << std::endl;
        }
        vm->notify();
    } catch (const unknown_option& exc) {
        std::cout << "Error parsing command line: " << exc.what() << std::endl;
    }
}

void get_configuration(std::shared_ptr<variables_map> vm) {
    // get values from config file
    std::ifstream config_ifs{"config.cfg"};
    if (!config_ifs.is_open()) {
        throw std::runtime_error("Error opening config file");
    }

    options_description config_options("Config File");
    config_options.add_options()
        ("host", value<std::string>()->default_value("localhost"), "Hostname to connect to")
        ("port", value<int>()->default_value(8042), "Port to connect")
        ("connection_timeout", value<int>()->default_value(5), "Timeout before closing a connection without data")
        ("max_file_size", value<int>()->default_value(8), "Maximum filesize in bytes")
        ("file_prefix", value<std::string>()->default_value("prefixo_"), "Filename prefix");

    store(parse_config_file(config_ifs, config_options), (*vm));
    vm->notify();
}

}