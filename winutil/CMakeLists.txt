cmake_minimum_required(VERSION 3.10)

project(winutil VERSION 0.0.1 LANGUAGES CXX)

add_compile_options(-std=c++23 -ggdb3 -O0)

option(BUILD_TESTS "Build test executables" OFF)

include_directories(include)

file(GLOB_RECURSE WINUTIL_SRC_FILES "${PROJECT_SOURCE_DIR}/src/winutil/*.cpp")
add_library(winutil SHARED ${WINUTIL_SRC_FILES})

if (BUILD_TESTS)
    find_package(Catch2 3 REQUIRED)

    set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")
    file(GLOB TEST_SRC_FILES "${TEST_DIR}/test_*.cpp")

    foreach(test_file ${TEST_SRC_FILES})
        get_filename_component(test_name ${test_file} NAME_WE)
        set(test_target "${test_name}")

        add_executable(${test_target} ${test_file})
        target_link_libraries(${test_target}
            PRIVATE
            winutil
            Catch2::Catch2WithMain
        )

        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()

    add_executable(test_runner_all ${TEST_SRC_FILES})
    target_link_libraries(test_runner_all
        PRIVATE
        winutil
        Catch2::Catch2WithMain
    )

    add_test(NAME test_all COMMAND test_all)

    set_target_properties(test_runner_all PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test_bin"
    )

    enable_testing()
endif()
