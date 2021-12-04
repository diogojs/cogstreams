cmake_minimum_required(VERSION 3.15)
project(cogstreams VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_RELEASE true)

# add_executable(cogstreams src/main.cpp)
# add_executable(cogclient src/main_client.cpp)
add_executable(cogserver)
target_sources(cogserver
    PRIVATE
        main_server.cpp
        helper.cpp
        helper.hpp
)

target_compile_options(cogserver
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)