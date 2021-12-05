#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
using namespace boost;

void testing(const system::error_code& e, asio::steady_timer* t, int* count) {
    std::cout << "testing called" << std::endl;

    if (*count < 5) {
        std::cout << "Called " << *count << " times." << std::endl;
        ++(*count);
        t->expires_at(t->expiry() + asio::chrono::seconds(1));

        t->async_wait(bind(testing, asio::placeholders::error, t, count));
    }

}

/*
    Utilitary just for convenience to run client and server at the same time
    in different processes.
*/
int main(int argc, char const *argv[]) {
    asio::io_context io_ctx;

    std::cout << "Running client and server...\n";

    int count{0};
    asio::steady_timer timer{io_ctx, asio::chrono::seconds(3)};
    std::cout << "Timer created\n";
    timer.async_wait(bind(testing, asio::placeholders::error, &timer, &count));

    io_ctx.run();

    std::cout << "Final count is " << count << std::endl;
    
    return 0;
}