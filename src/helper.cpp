#include <iostream>
#include <string>

#include "helper.hpp"

namespace cogs {

void handle_cli_options(int argc, char const *argv[]) {
    // Parse command line arguments
    if (argc == 1) {
        // Only program/binary name
        return;
    }

    // options: -h, --help
    auto opt_h = std::string{"-h"};
    if (opt_h.compare(argv[1]) == 0) {
        std::cout << "*** Usage ***\n"
            << "Client: client <port> <message>\n"
            << "Server: server <port>"
            << std::endl;
    }
}

}