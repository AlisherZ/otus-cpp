cmake_minimum_required(VERSION 3.9)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" OFF)
option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

add_executable(bayan main.cpp)
add_library(hash hash.cpp)
add_library(file_reader file_reader.cpp)
add_library(util util.cpp)
add_library(find_duplicate find_duplicate.cpp)

set_target_properties(bayan hash file_reader util find_duplicate PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

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

target_include_directories(hash
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(file_reader
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(util
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_include_directories(find_duplicate
    PRIVATE "${CMAKE_BINARY_DIR}"
)

find_package (Boost REQUIRED COMPONENTS program_options filesystem regex REQUIRED) 

include_directories (SYSTEM ${Boost_INCLUDE_DIR})

target_link_libraries(bayan PRIVATE
    hash
    file_reader
    util
    find_duplicate
    ${Boost_LIBRARIES}
)

target_link_libraries(find_duplicate PRIVATE
    hash
    file_reader
    util
    ${Boost_LIBRARIES}
)

target_link_libraries(file_reader PRIVATE
    hash
    ${Boost_LIBRARIES}
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_version test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(test_version
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        hash
        file_reader
        util
        find_duplicate
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
        hash
        file_reader
        util
        find_duplicate
        ${Boost_LIBRARIES}
    )
endif()

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(hash PRIVATE
        /W4
    )
    target_compile_options(file_reader PRIVATE
        /W4
    )
    target_compile_options(util PRIVATE
        /W4
    )
    target_compile_options(find_duplicate PRIVATE
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
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(hash PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(file_reader PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(util PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(find_duplicate PRIVATE
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

install(TARGETS bayan RUNTIME DESTINATION bin)

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
