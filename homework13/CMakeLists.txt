cmake_minimum_required(VERSION 3.15.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(fashio_mnist VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" OFF)
option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tensorflow)
    make_directory(${CMAKE_SOURCE_DIR}/tensorflow)
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tensorflow/lib/libtensorflow.so)
    file(
    DOWNLOAD
        https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-linux-x86_64-2.7.0.tar.gz ${CMAKE_SOURCE_DIR}/tensorflow/linux.tar.gz
    STATUS
        status
    LOG
        log
    )
    list(GET status 0 status_code)
    list(GET status 1 status_string)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading tensorflow lib: ${status_string}" "${log}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvzf ${CMAKE_SOURCE_DIR}/tensorflow/linux.tar.gz WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tensorflow)
    file(REMOVE ${CMAKE_SOURCE_DIR}/tensorflow/linux.tar.gz)
endif()

include_directories(tensorflow/include)
include_directories(src/3rdparty/scope_guard/include)
link_directories(${CMAKE_SOURCE_DIR}/tensorflow/lib)

add_executable(fashio_mnist mapper.cpp)

set_target_properties(fashio_mnist PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(fashio_mnist tensorflow Eigen3::Eigen)

if(WITH_GOOGLE_TEST)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_version test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )
endif()

if(WITH_GOOGLE_TEST)
    add_executable(
        hello_test
        hello_test.cc
    )
    target_link_libraries(
        hello_test
        GTest::gtest_main
    )
endif()

if (MSVC)
    target_compile_options(fashio_mnist PRIVATE
        /W4
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            /W4
        )
    endif()
    if(WITH_GOOGLE_TEST)
        target_compile_options(hello_test PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(fashio_mnist PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
    if(WITH_GOOGLE_TEST)
        target_compile_options(hello_test PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS fashio_mnist RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(test_version test_version)
endif()

if(WITH_GOOGLE_TEST)
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(hello_test)
endif()
