#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "helper.hpp"
#include "client.hpp"

int main(int argc, char const* argv[]) {
    using namespace boost::program_options;
    std::shared_ptr<variables_map> vm = std::make_shared<variables_map>();
    cogs::handle_cli_options(argc, argv, vm);

    try {
        cogs::get_configuration(vm);
        boost::asio::io_context io_context;

        auto config = (*vm);
        cogs::Client client{ io_context, config["host"].as<std::string>(), config["port"].as<int>() };
        client.send_messages();

        client.wait_answer();
    }
    catch (std::exception& exc) {
        std::cerr << "Exception: " << exc.what() << std::endl;
    }

    return 0;
}
