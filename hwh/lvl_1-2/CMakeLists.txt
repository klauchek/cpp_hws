cmake_minimum_required(VERSION 3.14)

project(dictionary)

set(SRC src/main.cpp)
set (EXEC_FILE "hwh_lvl_1-2")

add_executable(${EXEC_FILE} ${SRC})