cmake_minimum_required(VERSION 3.30)
project(Messanger)

set(CMAKE_CXX_STANDARD 20)

link_directories(lib)

add_subdirectory(lib)
add_subdirectory(bin)