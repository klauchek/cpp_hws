cmake_minimum_required(VERSION 3.14)

project(dictionary)

set(CMAKE_CXX_STANDARD 20)

set(LIB hashtable)

set(SRCS src/main.cpp)
set (EXEC_FILE "hwh_lvl_1-2")


add_executable(${EXEC_FILE} ${SRCS})

target_include_directories (${EXEC_FILE} PRIVATE ${CMAKE_SOURCE_DIR}/lib/${LIB})