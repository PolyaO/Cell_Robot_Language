cmake_minimum_required(VERSION 3.10)

project(debugger VERSION 0.0.1 LANGUAGES CXX)

include_directories(include)

file(GLOB_RECURSE DEBUGGER_SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(debugger SHARED ${DEBUGGER_SRC_FILES})
