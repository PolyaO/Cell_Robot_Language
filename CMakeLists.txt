cmake_minimum_required(VERSION 3.10)
project(RobotLng LANGUAGES CXX)
add_compile_options(-ggdb3 -O0 -fsanitize=address -std=c++23 -Wno-write-strings)
add_link_options(-fsanitize=address)
#add_definitions("-ggdb3 -O0 -fsanitize=address -std=c++23 -Wno-write-strings")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BISON_Parser_OUTPUTS
    ${PROJECT_SOURCE_DIR}/include/gen/parser.cpp
    ${PROJECT_SOURCE_DIR}/include/gen/parser.hpp
)
set(FLEX_Lexer_OUTPUTS
    ${PROJECT_SOURCE_DIR}/include/gen/lexer.cpp
    ${PROJECT_SOURCE_DIR}/include/gen/lexer.hpp
)

BISON_TARGET(Parser ${PROJECT_SOURCE_DIR}/src/interpreter/parse.yy
    ${PROJECT_SOURCE_DIR}/include/gen/parser.cpp
    DEFINES_FILE ${PROJECT_SOURCE_DIR}/include/gen/parser.hpp
             COMPILE_FLAGS "-Wcounterexamples")

FLEX_TARGET(Lexer src/interpreter/lexer.l
    ${PROJECT_SOURCE_DIR}/include/gen/lexer.cpp
    DEFINES_FILE ${PROJECT_SOURCE_DIR}/include/gen/lexer.hpp)

add_flex_bison_dependency(Lexer Parser)
 add_library(parser_lib SHARED
     ${BISON_Parser_OUTPUTS}
     ${FLEX_Lexer_OUTPUTS}
     src/interpreter/driver.cpp
     src/interpreter/ast_maker.cpp
 )

#add_executable(tst_variables tst_var.cpp src/backend/rvals/var_ops.cpp)
include_directories(include)
include_directories(include/gen)
include_directories(winutil/include)

add_subdirectory(winutil)

file(GLOB_RECURSE EXP_SOURCES "${PROJECT_SOURCE_DIR}/src/backend/exprs/*.cpp")
file(GLOB_RECURSE RVAL_SOURCES "${PROJECT_SOURCE_DIR}/src/backend/rvals/*.cpp")
file(GLOB_RECURSE INTERPR_SOURCES "${PROJECT_SOURCE_DIR}/src/interpreter/*.cpp")
file(GLOB_RECURSE ROBOT_SOURCES "${PROJECT_SOURCE_DIR}/src/robot/*.cpp")

file(GLOB_RECURSE VAR_SOURCES "${PROJECT_SOURCE_DIR}/src/backend/rvals/var_ops.cpp")


add_executable(tst_no_robot tst_no_robot.cpp ${EXP_SOURCES} ${RVAL_SOURCES} ${ROBOT_SOURCES} ${INTERPR_SOURCES})
add_executable(tst_maze winutil_test.cpp ${ROBOT_SOURCES} ${VAR_SOURCES})
add_executable(tst_file_view tst_file_view.cpp)
target_link_libraries(tst_no_robot PRIVATE winutil)
target_link_libraries(tst_no_robot PRIVATE parser_lib)
target_link_libraries(tst_maze PRIVATE winutil)
target_link_libraries(tst_file_view PRIVATE winutil)
