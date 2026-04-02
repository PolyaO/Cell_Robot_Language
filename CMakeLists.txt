cmake_minimum_required(VERSION 3.10)
project(RobotLng LANGUAGES CXX)
add_definitions("-ggdb3 -O0 -std=c++23 -Wno-write-strings")


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BISON_Parser_OUTPUTS
    include/gen/parser.cpp
    include/gen/parser.hpp
)
set(FLEX_Lexer_OUTPUTS
    include/gen/lexer.cpp
    include/gen/lexer.hpp
)

BISON_TARGET(Parser src/interpreter/parse.yy
    include/gen/parser.cpp
    DEFINES_FILE include/gen/parser.hpp
             COMPILE_FLAGS "-Wcounterexamples")

FLEX_TARGET(Lexer src/interpreter/lexer.l
    include/gen/lexer.cpp
    DEFINES_FILE include/gen/lexer.hpp)

add_flex_bison_dependency(Lexer Parser)
#add_library(parser_lib SHARED
#    ${BISON_Parser_OUTPUTS}
#    ${FLEX_Lexer_OUTPUTS}
#    src/interpreter/driver.cpp
#    src/interpreter/ast_maker.cpp
#)
#
#
#add_executable(tst_variables tst_var.cpp src/backend/rvals/var_ops.cpp)
include_directories(include)
include_directories(winutil/include)

add_subdirectory(winutil)

file(GLOB_RECURSE BACKEND_SOURCES "${PROJECT_SOURCE_DIR}/src/backend/*.cpp")
file(GLOB_RECURSE ROBOT_SOURCES "${PROJECT_SOURCE_DIR}/src/robot/*.cpp")

add_executable(tst_maze winutil_test.cpp ${BACKEND_SOURCES} ${ROBOT_SOURCES})
target_link_libraries(tst_maze PRIVATE winutil)
