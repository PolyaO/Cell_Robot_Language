cmake_minimum_required(VERSION 3.10)
project(ParserLib LANGUAGES CXX)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BISON_Parser_OUTPUTS
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
)
set(FLEX_Lexer_OUTPUTS
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.hpp
)

BISON_TARGET(Parser ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy
             ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
             COMPILE_FLAGS "-Wcounterexamples")

FLEX_TARGET(Lexer ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
            DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.hpp)

add_flex_bison_dependency(Lexer Parser)
add_library(parser_lib SHARED
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
    src/parse_driver.cpp
    src/ast_maker.cpp
)

target_include_directories(parser_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Те, кто использует библиотеку, тоже должны видеть эти заголовки
target_include_directories(parser_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

