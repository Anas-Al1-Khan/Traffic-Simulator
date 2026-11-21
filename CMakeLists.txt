cmake_minimum_required(VERSION 3.16)
project(TrafficSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Compiler-specific warning flags ---
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------
# Day 2: Qt6 GUI target
# ---------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()          # enables AUTOMOC (needed for Q_OBJECT)

qt_add_executable(traffic_sim
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
)
target_link_libraries(traffic_sim PRIVATE Qt6::Widgets)

# ---------------------------------------------------------------
# Day 1: queue tests
# ---------------------------------------------------------------
add_executable(test_queue tests/test_queue.cpp)
target_include_directories(test_queue PRIVATE src)