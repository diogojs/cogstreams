#ifndef COGSTREAM_HELPER_H
#define COGSTREAM_HELPER_H

#include <boost/program_options.hpp>

namespace cogs {

void handle_cli_options(int argc, char const *argv[], std::shared_ptr<boost::program_options::variables_map> vm);

void get_configuration(std::shared_ptr<boost::program_options::variables_map> vm);

const size_t max_msg_length{1024};
const char end_of_message{-1};

}

#endif