# cmake-format: off
# CMakeLists.txt -*-CMake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

cmake_minimum_required(VERSION 3.27)

project(
  beman.iterator_interface
  VERSION 0.0.0
  LANGUAGES CXX)

# Local helpers: required to include CompilerFeatureTest.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Includes
include(CTest)
include(FetchContent)
include(CompilerFeatureTest)

# Prechecks.
beman_iterator_check_deducing_this(COMPILER_SUPPORTS_DEDUCING_THIS)

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

option(BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS
  "Make use of C++23 \"deducing this\" feature (P0847R7). Turn this off for non-conforming compilers."
  ${COMPILER_SUPPORTS_DEDUCING_THIS})

option(BEMAN_ITERATOR_INTERFACE_BUILD_TESTING
       "Enable building tests and test infrastructure. Default: ON. Values: {ON, OFF}." ${PROJECT_IS_TOP_LEVEL})

option(BEMAN_ITERATOR_INTERFACE_BUILD_EXAMPLES
       "Enable building examples. Default: ON. Values: {ON, OFF}." ${PROJECT_IS_TOP_LEVEL})

if(BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS
   AND NOT COMPILER_SUPPORTS_DEDUCING_THIS)
  message(
    WARNING
      "Building with  C++23 \"deducing this\" feature (P0847R7) despite of the compiler's lack of actual support for it."
  )
endif()

configure_file(
  "${PROJECT_SOURCE_DIR}/include/beman/iterator_interface/config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/beman/iterator_interface/config.hpp" @ONLY)

if(BEMAN_ITERATOR_INTERFACE_BUILD_TESTING)
  # Fetch GoogleTest
  FetchContent_Declare(
    googletest
    EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG e39786088138f2749d64e9e90e0f9902daa77c40 # release-1.15.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

# Create the library target and named header set for beman.iterator_interface
add_library(beman.iterator_interface STATIC)
add_library(beman::iterator_interface ALIAS beman.iterator_interface)

target_sources(
  beman.iterator_interface
  PUBLIC FILE_SET
         beman_iterator_interface_headers
         TYPE
         HEADERS
         BASE_DIRS
         src
         include)

target_include_directories(
  beman.iterator_interface
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}>
)

add_subdirectory(src/beman/iterator_interface)
add_subdirectory(include/beman/iterator_interface)

if(BEMAN_ITERATOR_INTERFACE_BUILD_TESTING)
  add_subdirectory(tests/beman/iterator_interface)
endif()

if(BEMAN_ITERATOR_INTERFACE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Coverage
configure_file("cmake/gcovr.cfg.in" gcovr.cfg @ONLY)

add_custom_target(
  process_coverage
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Running gcovr to process coverage results"
  COMMAND mkdir -p coverage
  COMMAND gcovr --config gcovr.cfg .)

install(FILES ${PROJECT_BINARY_DIR}/include/beman/iterator_interface/config.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/beman/iterator_interface)
