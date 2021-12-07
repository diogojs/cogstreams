#ifndef COGSTREAM_HELPER_H
#define COGSTREAM_HELPER_H

namespace cogs {

void handle_cli_options(int argc, char const *argv[]);

const size_t max_msg_length{1024};
const std::string end_of_message{"end of message"};

}

#endif