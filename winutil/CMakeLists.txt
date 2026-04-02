cmake_minimum_required(VERSION 3.10)

project(winutil VERSION 0.0.1 LANGUAGES CXX)

include_directories(include)

file(GLOB_RECURSE WINUTIL_SRC_FILES "${PROJECT_SOURCE_DIR}/src/winutil/*.cpp")
add_library(winutil SHARED ${WINUTIL_SRC_FILES})
