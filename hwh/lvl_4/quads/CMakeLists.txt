
cmake_minimum_required(VERSION 3.14)

project(quads)

set(CMAKE_CXX_STANDARD 20)

option(PRINT_QUADS "PRINT_QUADS" OFF)

set(SRCS

    src/main.cpp
    src/strs_pair.cpp
)
set (EXEC_FILE "hwh_lvl_3")

add_executable(${EXEC_FILE} ${SRCS})

add_subdirectory(lib)
target_include_directories(${EXEC_FILE} PRIVATE include)
target_include_directories (${EXEC_FILE} PRIVATE ${CMAKE_SOURCE_DIR}/lib/include/${LIB})

if(PRINT_QUADS)
  add_compile_definitions(PRINT_QUADS)
endif()