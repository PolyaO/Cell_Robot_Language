cmake_minimum_required(VERSION 3.10)
project(RobotLng LANGUAGES CXX)

add_compile_options(-ggdb3 -O0 -std=c++23 -Wno-write-strings)

include_directories(winutil/include)
include_directories(var/include)
include_directories(interpreter/include)
include_directories(robot/include)

add_subdirectory(interpreter)
add_subdirectory(robot)
add_subdirectory(var)
add_subdirectory(winutil)

add_executable(tst_maze winutil_test.cpp)
add_executable(tst_no_robot tst_no_robot.cpp)
add_executable(tst_file_view tst_file_view.cpp)
add_executable(tst_window_resize tst_window_resize.cpp)
target_link_libraries(tst_maze PRIVATE winutil var interpreter robot)
target_link_libraries(tst_no_robot PRIVATE winutil var interpreter)
target_link_libraries(tst_file_view PRIVATE winutil)
target_link_libraries(tst_window_resize PRIVATE winutil)
