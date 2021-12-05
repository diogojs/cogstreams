#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "helper.hpp"

namespace cogs {

void handle_cli_options(int argc, char const *argv[]) {
    using namespace boost;

    try {
        program_options::options_description description("Allowed options");
        description.add_options()
            ("help,h", "Show this help message");

        program_options::variables_map vm;
        program_options::store(program_options::parse_command_line(argc, argv, description), vm);

        if (vm.count("help")) {
            std::cout << "*** Usage ***\n"
                << "Client: cogclient <port> <messages>\n"
                << "Server: cogserver <port>"
                << std::endl;
        }

    } catch (const boost::program_options::unknown_option& exc) {
        std::cout << "Error parsing command line: " << exc.what() << std::endl;
    }
}

}