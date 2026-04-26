cmake_minimum_required(VERSION 3.10)

project(robot VERSION 0.0.1 LANGUAGES CXX)

include_directories(include)

file(GLOB_RECURSE ROBOT_SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(robot SHARED ${ROBOT_SRC_FILES})

add_executable(mazegen mazegen.cpp src/maze.cpp)
