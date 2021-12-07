#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "helper.hpp"

namespace cogs {
using namespace boost;


class Client {
public:
    Client(asio::io_context& io_ctx, int port)
        : io_ctx_{ io_ctx }, port_{ port }, socket_{ io_ctx }
    {
        asio::ip::tcp::resolver resolver{ io_ctx };
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(std::string{ "localhost" }, std::to_string(port));

        std::cout << "Will connect" << std::endl;
        asio::connect(this->socket_, endpoints);
        std::cout << "connected" << std::endl;
    }

    void send_message() {
        std::cout << "Enter message (leave it empty to quit): ";
        // std::string request{"my message with spaces"};
        std::string request;
        do {
            getline(std::cin, request);
            auto request_length = request.size();
            std::cout << "read " << request_length << " bytes" << std::endl;
            asio::write(this->socket_, asio::buffer(request, request_length));
        } while (request != "");

        asio::write(this->socket_, asio::buffer(cogs::end_of_message));
    }

    void wait_answer() {
        char reply[cogs::max_msg_length];
        this->socket_.read_some(asio::buffer(reply));
        std::cout << "Server answer: ";
        std::cout << reply << std::endl;
    }

private:
    asio::io_context& io_ctx_;

    int port_;
    asio::ip::tcp::socket socket_;
};

} // namespace cogs

int main(int argc, char const* argv[]) {
    // usage: client <port> <message>
    cogs::handle_cli_options(argc, argv);
    int port = 8013;

    try {
        boost::asio::io_context io_context;

        std::cout << "Instantiating Client..." << std::endl;
        cogs::Client client{ io_context, port };
        std::cout << "Instantiated Client." << std::endl;
        client.send_message();

        std::cout << "Waiting for confirmation from server..." << std::endl;
        client.wait_answer();
    }
    catch (std::exception& exc) {
        std::cerr << "Exception: " << exc.what() << std::endl;
    }

    return 0;
}
