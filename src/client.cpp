#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "helper.hpp"
#include "client.hpp"

namespace cogs {
using namespace boost;

Client::Client(asio::io_context& io_ctx, int port)
    : io_ctx_{ io_ctx }, port_{ port }, socket_{ io_ctx }
{
    asio::ip::tcp::resolver resolver{ io_ctx };
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(std::string{ "localhost" }, std::to_string(port));

    asio::connect(this->socket_, endpoints);
}

void Client::send_messages() {
    std::cout << "Enter message (leave it empty to quit): ";
    std::string request;
    do {
        getline(std::cin, request);
        auto request_length = request.size();
        std::cout << "Sending " << request_length << " bytes" << std::endl;
        asio::write(this->socket_, asio::buffer(request, request_length));
    } while (request != "");

    asio::write(this->socket_, asio::buffer(&cogs::end_of_message, 1));
}

void Client::wait_answer() {
    char reply[cogs::max_msg_length];
    this->socket_.read_some(asio::buffer(reply));
    std::cout << "Server answer: ";
    std::cout << reply << std::endl;
}

} // namespace cogs