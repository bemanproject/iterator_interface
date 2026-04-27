# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.30...4.3)

project(
    beman.iterator_interface # CMake Project Name, which is also the name of the top-level
    # targets (e.g., library, executable, etc.).
    DESCRIPTION "iterator creation mechanisms"
    LANGUAGES CXX
    VERSION 0.1.0
)

# Local helpers: required to include CompilerFeatureTest.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Includes
include(CompilerFeatureTest)

# Prechecks.
beman_iterator_check_deducing_this(COMPILER_SUPPORTS_DEDUCING_THIS)

option(
    BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS
    "Make use of C++23 \"deducing this\" feature (P0847R7). Turn this off for non-conforming compilers."
    ${COMPILER_SUPPORTS_DEDUCING_THIS}
)

# [CMAKE.SKIP_TESTS]
option(
    BEMAN_ITERATOR_INTERFACE_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# [CMAKE.SKIP_EXAMPLES]
option(
    BEMAN_ITERATOR_INTERFACE_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

if(
    BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS
    AND NOT COMPILER_SUPPORTS_DEDUCING_THIS
)
    message(
        WARNING
        "Building with C++23 \"deducing this\" feature (P0847R7) despite of the compiler's lack of actual support for it."
    )
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/include/beman/iterator_interface/config.hpp.in"
    "${PROJECT_BINARY_DIR}/include/beman/iterator_interface/config.hpp"
    @ONLY
)

# for find of beman_install_library and configure_build_telemetry
include(infra/cmake/beman-install-library.cmake)
include(infra/cmake/BuildTelemetryConfig.cmake)

add_library(beman.iterator_interface INTERFACE)
add_library(beman::iterator_interface ALIAS beman.iterator_interface)

target_sources(
    beman.iterator_interface
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS
                "${CMAKE_CURRENT_SOURCE_DIR}/include"
                "${PROJECT_BINARY_DIR}/include"
)

set_target_properties(
    beman.iterator_interface
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ${PROJECT_IS_TOP_LEVEL}
)

add_subdirectory(include/beman/iterator_interface)

beman_install_library(beman.iterator_interface TARGETS beman.iterator_interface)
configure_build_telemetry()

if(BEMAN_ITERATOR_INTERFACE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/beman/iterator_interface)
endif()

if(BEMAN_ITERATOR_INTERFACE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
