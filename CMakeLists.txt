cmake_minimum_required(VERSION 3.5)

project(PZMiner)
set(PROJECT_VERSION 0.2.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -pedantic -Wno-comment")

add_executable(pzcoin hashing/main.cpp hashing/ec.cpp)

target_link_libraries(pzcoin gmp)
