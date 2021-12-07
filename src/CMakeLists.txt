cmake_minimum_required(VERSION 3.15)
project(cogstreams VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_RELEASE true)

find_package(Boost REQUIRED
    COMPONENTS program_options
)

# === General === #
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

# === Client === #
add_executable(cogclient)
target_sources(cogclient
    PRIVATE
        main_client.cpp
        helper.cpp
        helper.hpp
)

if(Boost_FOUND)
    target_include_directories(cogclient
        PRIVATE
            ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(cogclient
        PRIVATE
            ${Boost_LIBRARIES}
            pthread
            boost_system
    )
endif()

target_compile_options(cogclient
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# === Server === #
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