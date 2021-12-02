#include <iostream>
#include "helper.hpp"

int main(int argc, char const *argv[]) {
    // usage: client <port> <message>
    using namespace std::string_literals;
    using namespace cogs;

    cogs::handle_cli_options(argc, argv);

    // get values from config file

    // create socket connection on port <port>
    // wait for connection to complete

    // send message

    // receive answer from server

    return 0;
}