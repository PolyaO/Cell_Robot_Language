cmake_minimum_required(VERSION 3.10)
project(RobotLng LANGUAGES CXX)
add_definitions("-ggdb3 -O0 -std=c++23 -Wno-write-strings")


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BISON_Parser_OUTPUTS
    src/gen/parser.cpp
    include/gen/parser.hpp
)
set(FLEX_Lexer_OUTPUTS
    src/gen/lexer.cpp
    include/gen/lexer.hpp
)

BISON_TARGET(Parser src/interpreter/parse.yy
    src/gen/parser.cpp
    DEFINES_FILE /include/parser.hpp
             COMPILE_FLAGS "-Wcounterexamples")

FLEX_TARGET(Lexer /lexer.l
    /src/lexer.cpp
    DEFINES_FILE /include/lexer.hpp)

add_flex_bison_dependency(Lexer Parser)
add_library(parser_lib SHARED
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
    src/parse_driver.cpp
    src/ast_maker.cpp
)

target_include_directories(parser_lib PRIVATE
    /include
    /../include

)

# Те, кто использует библиотеку, тоже должны видеть эти заголовки
target_include_directories(parser_lib INTERFACE
    /include
    /../include
)


add_executable(tst_variables tst_var.cpp src/backend/rvals/var_ops.cpp)
include_directories(include)
