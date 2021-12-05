cmake_minimum_required(VERSION 3.15)
project(cogstreams VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_RELEASE true)

find_package(Boost REQUIRED
    COMPONENTS program_options
)

add_executable(cogstreams)
target_sources(cogstreams
    PRIVATE
        main.cpp
)

if(Boost_FOUND)
    target_include_directories(cogstreams
        PRIVATE
            ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(cogstreams
        PRIVATE
            ${Boost_LIBRARIES}
            pthread
            boost_system
    )
endif()


# add_executable(cogclient src/main_client.cpp)

# === Server ===
add_executable(cogserver)
target_sources(cogserver
    PRIVATE
        main_server.cpp
        helper.cpp
        helper.hpp
)

if(Boost_FOUND)
    target_include_directories(cogserver
        PRIVATE
            ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(cogserver
        PRIVATE
            ${Boost_LIBRARIES}
            pthread
            boost_system
    )
endif()

target_compile_options(cogserver
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)