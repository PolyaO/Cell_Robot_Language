cmake_minimum_required(VERSION 3.10)
project(RobotLng LANGUAGES CXX)

add_subdirectory(parser)
add_executable(main_exe main.cpp)
target_link_libraries(main_exe PRIVATE parser_lib)
