#include <iostream>
#include <ctime>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include "server.hpp"

namespace cogs
{
using namespace boost;

std::string make_timestamp()
{
    std::time_t now{ std::time(0) };
    auto time = localtime(&now);
    return str(format("%04d%02d%02d%02d%02d%02d") % (time->tm_year + 1900) % (time->tm_mon + 1) % time->tm_mday % time->tm_hour % time->tm_min % time->tm_sec);
}

void TCPConnection::start()
{
    this->async_read();
}

void TCPConnection::async_read() {
    memset(this->received_buffer_, 0, sizeof(this->received_buffer_));
    this->socket_.async_read_some(asio::buffer(this->received_buffer_),
        bind(&TCPConnection::handle_read, shared_from_this(),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred));
}

void TCPConnection::handle_read(const system::error_code error, size_t bytes_received) {
    if (error) {
        std::cout << "error: " << error << std::endl;
    }
    std::cout << "Received:" << received_buffer_ << "END" << std::endl;

    if (this->received_buffer_[0] == cogs::end_of_message) {
        // End of connection
        std::string reply{ "Data saved succesfully." };
        std::cout << reply << std::endl;
        asio::write(this->socket_, asio::buffer(reply, reply.size()));
    }
    else {
        std::cout << "bytes received: " << bytes_received << std::endl;
        auto filename = this->get_filename();
        this->write_file(filename, this->received_buffer_, bytes_received);

        this->async_read();
    }
}

void TCPConnection::write_file(std::string filename, const char* data, size_t bytes_received) {
    auto current_file = std::fstream(filename, std::ios::out | std::ios::app);

    auto current_filesize = filesystem::file_size(filename);
    auto available_space = this->max_file_size_ - current_filesize;

    if (available_space >= bytes_received) {
        current_file << data;
    }
    else {
        current_file.write(data, available_space);

        // Write the rest of the data to a new file
        auto filename = this->get_filename(true);
        this->write_file(filename, &data[available_space], bytes_received - available_space);
    }
    current_file.close();
}

std::string TCPConnection::get_filename(bool needs_new_file) {
    int counter = 0;
    if (!needs_new_file && filesystem::exists(this->current_filename)) {
        return this->current_filename;
    }
    auto timestamp = make_timestamp();
    this->current_filename = this->filename_prefix_ + timestamp + "_" + std::to_string(counter);
    while (filesystem::exists(this->current_filename)) {
        counter++;
        this->current_filename = this->filename_prefix_ + timestamp + "_" + std::to_string(counter);
    }
    return this->current_filename;
}

TCPConnection::~TCPConnection() {
    auto filename = this->get_filename();
    if (filesystem::exists(filename)) {

    }
}

void Server::start_accept() {
    std::cout << "Listening to port " << this->port_ << "..." << std::endl;
    shared_ptr<TCPConnection> connection = TCPConnection::create(this->io_ctx_, this->port_, this->connection_timeout_, this->max_file_size_, filename_prefix_);
    this->acceptor_.async_accept(connection->socket(), bind(&Server::handle_accept, this, connection, asio::placeholders::error));
}

void Server::handle_accept(shared_ptr<TCPConnection> connection, const system::error_code& error) {
    if (!error) {
        connection->start();
    }

    start_accept();
}

} // namespace cogs
