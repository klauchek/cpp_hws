
cmake_minimum_required(VERSION 3.14)

project(quads)

set(CMAKE_CXX_STANDARD 20)

option(PRINT_QUADS "PRINT_QUADS" OFF)
option(WITH_TESTS "ENABLE UNIT TESTS" OFF)

set(LIB hashtable)
set(SRCS

    src/main.cpp
    src/strs_pair.cpp
)
set (HEADER include/strs_pair.h)
set (EXEC_FILE "hwh_lvl_3")

# add_subdirectory(lib/hashtable)
# target_include_directories(${LIB} INTERFACE
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/hashtable>
#   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # <prefix>/include/mylib
# )

add_executable(${EXEC_FILE} ${SRCS})
target_include_directories(${EXEC_FILE} PRIVATE include)
target_include_directories (${EXEC_FILE} PRIVATE ${CMAKE_SOURCE_DIR}/lib/${LIB})

if(PRINT_QUADS)
  add_compile_definitions(PRINT_QUADS)
endif()

if (WITH_TESTS)
    message("building with google tests")
    enable_testing()
    add_subdirectory(lib)
endif()