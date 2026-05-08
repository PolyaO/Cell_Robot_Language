cmake_minimum_required(VERSION 3.10)
project(RobotLng LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    winutil
    GIT_REPOSITORY https://github.com/Roma004/winutil.git
    GIT_TAG master
)

FetchContent_MakeAvailable(winutil)

add_compile_options(-ggdb3 -O0 -std=c++23 -Wno-write-strings)

include_directories(var/include)
include_directories(interpreter/include)
include_directories(robot/include)
include_directories(debugger/include)
include_directories(${winutil_SOURCE_DIR}/include)

add_subdirectory(interpreter)
add_subdirectory(robot)
add_subdirectory(var)
add_subdirectory(debugger)

add_executable(tst_maze tst_maze.cpp)
add_executable(debug_ideal_robot debug_robot.cpp)
target_compile_definitions(debug_ideal_robot PRIVATE USE_IDEAL_ROBOT)
add_executable(debug_paranoid_robot debug_robot.cpp)
target_compile_definitions(debug_paranoid_robot PRIVATE USE_PARANOID)

target_link_libraries(tst_maze PRIVATE winutil var interpreter robot)
target_link_libraries(debug_ideal_robot PRIVATE winutil var interpreter robot debugger)
target_link_libraries(debug_paranoid_robot PRIVATE winutil var interpreter robot debugger)
