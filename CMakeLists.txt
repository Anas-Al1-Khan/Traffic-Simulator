cmake_minimum_required(VERSION 3.16)
project(TrafficSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(test_queue tests/test_queue.cpp)
target_include_directories(test_queue PRIVATE src)