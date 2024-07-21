cmake_minimum_required(VERSION 3.9)
project(async)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set (BUILD_SHARED_LIBS TRUE)
endif ()

add_library(async SHARED async.cpp async.h)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)