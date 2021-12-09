#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "helper.hpp"
#include "client.hpp"

namespace cogs {
using namespace boost;

Client::Client(asio::io_context& io_ctx, std::string host, int port)
    : io_ctx_{ io_ctx }, host_{host}, port_{ port }, socket_{ io_ctx }
{
    asio::ip::tcp::resolver resolver{ io_ctx };
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

    asio::connect(this->socket_, endpoints);
}

void Client::send_messages() {
    std::cout << "Enter message (leave it empty to quit):\n";
    std::string request;
    do {
        getline(std::cin, request);
        auto request_length = request.size();
        check_connection();
        if (!this->socket_.is_open()) return;
        std::cout << "Sending " << request_length << " bytes" << std::endl;
        asio::write(this->socket_, asio::buffer(request, request_length));
    } while (request != "");

    asio::write(this->socket_, asio::buffer(&cogs::end_of_message, 1));
}

void Client::wait_answer() {
    if (this->socket_.is_open()) {
        char reply[cogs::max_msg_length] = {0};
        this->socket_.read_some(asio::buffer(reply));
        std::cout << "Server answer: ";
        std::cout << reply << std::endl;

        // For this example project the server only sends data if the connection is closing for some reason
        this->socket_.close();
    }
}

void Client::check_connection() {
    if (this->socket_.available()) {
       wait_answer(); 
    }
}

} // namespace cogs
