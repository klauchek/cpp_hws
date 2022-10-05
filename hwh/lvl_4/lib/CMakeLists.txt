cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)

set(LIB hashtable)
project(${LIB})

add_library(${LIB} INTERFACE)

target_include_directories(${LIB}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

option(WITH_TESTS "ENABLE UNIT TESTS" OFF)

if (WITH_TESTS)
    message("building with google tests")
    enable_testing()
    add_subdirectory(unit_tests)
endif()

set(LIB "${LIB}" PARENT_SCOPE)