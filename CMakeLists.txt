cmake_minimum_required(VERSION 3.5)

project(PZMiner)
set(PROJECT_VERSION 0.3.1)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wextra -pedantic -Wno-comment")

add_library(ec SHARED hashing/ec.cpp)

add_library(ecoh SHARED hashing/ecoh.cpp)

add_executable(pzcoin hashing/main.cpp)

target_link_libraries(pzcoin gmp)

target_link_libraries(ecoh ec)

target_link_libraries(pzcoin ecoh)
