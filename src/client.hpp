#ifndef COGSTREAM_CLIENT_H
#define COGSTREAM_CLIENT_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "helper.hpp"

namespace cogs {
using namespace boost;


class Client {
public:
    Client(asio::io_context& io_ctx, int port);

    void send_messages();

    void wait_answer();

private:
    asio::io_context& io_ctx_;

    int port_;
    asio::ip::tcp::socket socket_;
};

} // namespace cogs

#endif