#include <iostream>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "helper.hpp"

namespace cogs
{
using namespace boost;

std::string make_daytime_string()
{
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

class TCPConnection
    : public enable_shared_from_this<TCPConnection>
{
public:
    static shared_ptr<TCPConnection> create(asio::io_context& io_context)
    {
        return shared_ptr<TCPConnection>(new TCPConnection(io_context));
    }

    asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        this->async_read();
    }

private:
    TCPConnection(asio::io_context& io_ctx)
        : socket_(io_ctx) {}

    void async_read() {
        this->socket_.async_read_some(asio::buffer(this->received_buffer_),
            bind(&TCPConnection::handle_read, shared_from_this(),
                asio::placeholders::error,
                asio::placeholders::bytes_transferred));
    }

    void handle_read(const system::error_code error, size_t bytes_received) {
        if (error) {
            std::cout << "error: " << error << std::endl;
        }
        std::cout << "bytes received: " << bytes_received << std::endl;

        if (strcmp(this->received_buffer_, cogs::end_of_message.c_str()) == 0) {
            std::string reply{ "Data saved succesfully." };
            std::cout << reply << std::endl;
            asio::write(this->socket_, asio::buffer(reply, reply.size()));
        }
        else {
            this->async_read();
        }
    }

    asio::ip::tcp::socket socket_;
    char received_buffer_[cogs::max_msg_length];
};

class Server {
public:
    Server(asio::io_context& io_ctx, int port, int connection_timeout, int max_file_size, const std::string& filename_prefix)
        : io_ctx_{ io_ctx }, acceptor_{ io_ctx_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port) },
        port_{ port }, connection_timeout_{ connection_timeout }, max_file_size_{ max_file_size }, filename_prefix_{ filename_prefix }
    {
        start_accept();
    }

private:
    void start_accept() {
        std::cout << "Listening..." << std::endl;
        shared_ptr<TCPConnection> connection = TCPConnection::create(this->io_ctx_);
        this->acceptor_.async_accept(connection->socket(), bind(&Server::handle_accept, this, connection, asio::placeholders::error));
    }

    void handle_accept(shared_ptr<TCPConnection> connection, const system::error_code& error) {
        if (!error) {
            connection->start();
        }

        start_accept();
    }

    asio::io_context& io_ctx_;
    asio::ip::tcp::acceptor acceptor_;

    int port_;
    int connection_timeout_;
    int max_file_size_;
    std::string filename_prefix_;
};

} // namespace cogs

int main(int argc, char const* argv[]) {
    // usage: client <port> <message>
    using namespace std::string_literals;

    cogs::handle_cli_options(argc, argv);

    // get values from config file
    std::string config_filename{ "config.cfg" };

    int port = 8013;
    int connection_timeout = 5;
    int max_file_size = 32;
    std::string filename_prefix{ "data_" };

    try {
        boost::asio::io_context io_context;

        std::cout << "Instantiating Server..." << std::endl;
        cogs::Server server{ io_context, port, connection_timeout, max_file_size, filename_prefix };
        std::cout << "Server instantiated." << std::endl;

        io_context.run();
    }
    catch (std::exception& exc) {
        std::cerr << "Exception: " << exc.what() << std::endl;
    }

    return 0;
}
