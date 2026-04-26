cmake_minimum_required(VERSION 3.10)

project(var VERSION 0.0.1 LANGUAGES CXX)

include_directories(include)

file(GLOB_RECURSE VAR_SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(var SHARED ${VAR_SRC_FILES})
