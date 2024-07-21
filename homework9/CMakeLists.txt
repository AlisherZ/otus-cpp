cmake_minimum_required(VERSION 3.9)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(async ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set (BUILD_SHARED_LIBS TRUE)
endif ()

add_library(async SHARED async.cpp async.h)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)

install(TARGETS async_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
