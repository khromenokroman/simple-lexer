cmake_minimum_required(VERSION 3.28)
project(untitled19)
find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled19 main.cpp)

target_link_libraries(untitled19 PRIVATE fmt::fmt)
