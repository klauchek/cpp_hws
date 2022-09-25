cmake_minimum_required(VERSION 3.14)

project(dictionary)

set(LIB hashtable)

add_subdirectory(lib)

target_include_directories(${LIB} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(SRC src/main.cpp)
set (EXEC_FILE "hwh_lvl_1-2")

add_executable(${EXEC_FILE} ${SRC})
target_include_directories(${EXEC_FILE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/${LIB})
target_link_libraries(${EXEC_FILE} PUBLIC ${LIB})