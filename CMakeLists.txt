cmake_minimum_required(VERSION 3.16)
project(TrafficSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(traffic_sim
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/models.h
    src/vehicleitem.h
    src/vehicleitem.cpp
    src/Graph.cpp
    src/Graph.h
    src/Simulation.cpp
    src/Simulation.h
)
target_link_libraries(traffic_sim PRIVATE Qt6::Widgets)

add_executable(test_queue tests/test_queue.cpp)
target_include_directories(test_queue PRIVATE src)