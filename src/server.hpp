#ifndef COGSTREAM_SERVER_H
#define COGSTREAM_SERVER_H

#include <iostream>
#include <ctime>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "helper.hpp"

namespace cogs
{
using namespace boost;

std::string make_daytime_string();

class TCPConnection
    : public enable_shared_from_this<TCPConnection>
{
public:
    static shared_ptr<TCPConnection> create(asio::io_context& io_context, int port, int connection_timeout, int max_file_size, const std::string& filename_prefix)
    {
        return shared_ptr<TCPConnection>(new TCPConnection(io_context, port, connection_timeout, max_file_size, filename_prefix));
    }

    ~TCPConnection();

    asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start();

private:
    TCPConnection(asio::io_context& io_ctx, int port, int connection_timeout, int max_file_size, const std::string& filename_prefix)
        : socket_(io_ctx), port_{ port }, connection_timeout_{ connection_timeout }, max_file_size_{ max_file_size }, filename_prefix_{ filename_prefix }
    {}


    void async_read();
    void handle_read(const system::error_code error, size_t bytes_received);
    std::string get_filename(bool needs_new_file=false);
    void write_file(std::string filename, const char* data, size_t bytes_received);

    asio::ip::tcp::socket socket_;
    int port_;
    int connection_timeout_;
    int max_file_size_;
    std::string filename_prefix_;
    char received_buffer_[cogs::max_msg_length];
    
    std::string current_filename;
    std::fstream file;
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
    void start_accept();

    void handle_accept(shared_ptr<TCPConnection> connection, const system::error_code& error);

    asio::io_context& io_ctx_;
    asio::ip::tcp::acceptor acceptor_;

    int port_;
    int connection_timeout_;
    int max_file_size_;
    std::string filename_prefix_;
};

} // namespace cogs

#endif