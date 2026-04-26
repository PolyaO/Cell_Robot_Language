cmake_minimum_required(VERSION 3.10)
project(interpreter LANGUAGES CXX)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(include)
include_directories(include/gen)

set(PARSER_NAME ${PROJECT_SOURCE_DIR}/include/gen/parser)
set(LEXER_NAME  ${PROJECT_SOURCE_DIR}/include/gen/lexer)

set(BISON_Parser_OUTPUTS ${PARSER_NAME}.cpp ${PARSER_NAME}.hpp)
set(FLEX_Lexer_OUTPUTS   ${LEXER_NAME}.cpp  ${LEXER_NAME}.hpp )

BISON_TARGET(parser ${PROJECT_SOURCE_DIR}/src/parse.yy
    ${PARSER_NAME}.cpp DEFINES_FILE ${PARSER_NAME}.hpp
             COMPILE_FLAGS "-Wcounterexamples")

FLEX_TARGET(lexer src/lexer.l ${LEXER_NAME}.cpp DEFINES_FILE ${LEXER_NAME}.hpp)
add_flex_bison_dependency(lexer parser)

file(GLOB_RECURSE INTERP_SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(interpreter SHARED
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
    ${INTERP_SRC_FILES}
)
