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
    ExternalProject_Add(wavelet_denoise
      GIT_REPOSITORY  https://github.com/rafat/wavelib.git
      INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/wavelet_denoise1/install
    )
    # INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/fftw/install/include)
    add_library(wavelet wavelet_denoise.c wavelet_denoise.h)
    add_dependencies(wavelet wavelet_denoise)
    # add_library(wdenoise SHARED IMPORTED)
    # set_property(TARGET wdenoise PROPERTY IMPORTED_IMPLIB "../wdenoise/build/Bin/wdenoiselib.lib")
    # set_target_properties(wdenoise PROPERTIES 
    #     LINKER_LANGUAGE C
    #     C_STANDARD 99
    #     C_STANDARD_REQUIRED ON)
    add_executable(ECG main.cpp message.pb.cc message.pb.h)
    # add_library(wavelet2d SHARED IMPORTED)
    # set_property(TARGET wavelet2d PROPERTY IMPORTED_LOCATION "../wavelet2d.dll")
    # set_property(TARGET wavelet2d PROPERTY IMPORTED_IMPLIB "../wavelet2d.lib")
    target_link_libraries(ECG PRIVATE wavelet ${Boost_LIBRARIES})
else ()
    # add_subdirectory(wavelib)
    find_package(Protobuf REQUIRED)
    include_directories(${Protobuf_INCLUDE_DIRS})
    include(CMakePrintHelpers)
    cmake_print_variables(PROTO_SRCS)
    cmake_print_variables(PROTO_HDRS)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/message.proto)
    cmake_print_variables(PROTO_SRCS)
    cmake_print_variables(PROTO_HDRS)
    include(ExternalProject)
    ExternalProject_Add(wavelet_denoise
      GIT_REPOSITORY  https://github.com/rafat/wavelib.git
      INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/wavelet_denoise/install
    )
    add_executable(ECG main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
    add_library(wavelet wavelet_denoise.c)
    add_dependencies(wavelet wavelet_denoise)
#    add_library(wavelet_denoise SHARED IMPORTED)
#    set_property(TARGET wavelet_denoise PROPERTY IMPORTED_IMPLIB "wdenoise/Bin/libwdenoiselib.a")
#    set_target_properties(wavelet PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(ECG PRIVATE wavelet fftw3 ${Protobuf_LIBRARIES} ${Boost_LIBRARIES})
    # target_link_libraries(wavelet wdenoiselib)
endif()

set_target_properties(ECG PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(wavelet PROPERTIES
    LINKER_LANGUAGE C
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(ECG PRIVATE
        /W4
    )
else ()
    target_compile_options(ECG PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ECG RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
