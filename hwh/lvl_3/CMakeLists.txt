cmake_minimum_required(VERSION 3.14)

project(quads)

option(PRINT_QUADS "PRINT_QUADS" OFF)

set(SRCS

    src/main.cpp
    src/strs_pair.cpp
)
set (HEADER include/strs_pair.hpp)
set (EXEC_FILE "hwh_lvl_3")

add_executable(${EXEC_FILE} ${SRCS})
target_include_directories(${EXEC_FILE} PRIVATE include)

if(PRINT_QUADS)
  add_compile_definitions(PRINT_QUADS)
endif()