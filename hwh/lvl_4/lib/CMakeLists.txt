# project_lib
# ├── CMakeLists.txt
# ├── include
# │   └── project
# │       ├── interface.h
# │       ├── line.hpp
# ├── src
# │   ├── interface.cc
# │   └── PowOfTwo.h
# └── test
#     ├── CMakeLists.txt
#     └── main.cc

# по идее - верхний CMakeLists создаёт создаёт все свойства либы, прописывает пути, компилирует объектники и тд.
# Если указать что нужны тесты - то add_subdirectory(test) или у cmake есть прямо директива gtest, не помню.

cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME hashtable)
set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME})
set(LIB hashtable)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
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