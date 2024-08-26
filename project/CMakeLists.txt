cmake_minimum_required(VERSION 3.15.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ECG VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" OFF)
option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

if (MSVC)
    # add_subdirectory(wdenoise)
    # add_library(wdenoise SHARED wdenoise)
    # set_target_properties(Test1 PROPERTIES LINKER_LANGUAGE C)
    include(ExternalProject)
    set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
    ExternalProject_Add(wavelet_denoise
      GIT_REPOSITORY  https://github.com/rafat/wavelib.git
      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    )
    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
    add_library(wavelet wavelet_denoise.c)
    add_dependencies(wavelet wavelet_denoise)
    target_link_libraries(wavelet PRIVATE wauxlib wavelib)
    add_executable(ECG main.cpp message.pb.cc message.pb.h)
    add_dependencies(ECG wavelet_denoise)
    target_link_libraries(ECG PRIVATE wavelet wauxlib wavelib ${Boost_LIBRARIES})
else ()
    find_package(Protobuf REQUIRED)
    include_directories(${Protobuf_INCLUDE_DIRS})
    include(CMakePrintHelpers)
    cmake_print_variables(PROTO_SRCS)
    cmake_print_variables(PROTO_HDRS)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/message.proto proto/filter.proto)
    cmake_print_variables(PROTO_SRCS)
    cmake_print_variables(PROTO_HDRS)
    
    include(ExternalProject)
    set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
    ExternalProject_Add(wavelet_denoise
      GIT_REPOSITORY  https://github.com/rafat/wavelib.git
      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    )
    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
    
    add_library(wavelet wavelet_denoise.c)
    add_dependencies(wavelet wavelet_denoise)
    target_link_libraries(wavelet PRIVATE wauxlib wavelib)

    add_library(wavelet_api wavelet_cpp_api.cpp ${PROTO_SRCS} ${PROTO_HDRS})
    add_dependencies(wavelet_api wavelet_denoise)
    target_link_libraries(wavelet_api PRIVATE wavelet wauxlib wavelib)

    add_library(fft_api fft_api.cpp ${PROTO_SRCS} ${PROTO_HDRS})

    add_executable(ECG main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
    add_dependencies(ECG wavelet_denoise)
    target_link_libraries(ECG PRIVATE fftw3 wavelet wauxlib wavelib ${Protobuf_LIBRARIES} ${Boost_LIBRARIES})
endif()

set_target_properties(ECG PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(wavelet PROPERTIES
    LINKER_LANGUAGE CXX
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
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
    target_compile_options(ECG PRIVATE
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
    target_compile_options(ECG PRIVATE
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

install(TARGETS ECG RUNTIME DESTINATION bin)

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
