# cmake-format: off
# CMakeLists.txt -*-CMake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

cmake_minimum_required(VERSION 3.27)

project(
  beman_iterator_interface
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
       "Make use of deducing this. Turn this off for non-conforming compilers."
       ${COMPILER_SUPPORTS_DEDUCING_THIS})

option(ITERATOR_INTERFACE_ENABLE_TESTING
       "Enable building tests and test infrastructure" ${PROJECT_IS_TOP_LEVEL})

if(BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS
   AND NOT COMPILER_SUPPORTS_DEDUCING_THIS)
  message(
    WARNING
      "Building with deducing this support despite of the compiler's lack of support for it"
  )
endif()

configure_file(
  "${PROJECT_SOURCE_DIR}/include/beman/iterator_interface/config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/beman/iterator_interface/config.hpp" @ONLY)

# Build the tests if enabled via the option ITERATOR_INTERFACE_ENABLE_TESTING
if(ITERATOR_INTERFACE_ENABLE_TESTING)
  # Fetch GoogleTest
  FetchContent_Declare(
    googletest
    EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG e39786088138f2749d64e9e90e0f9902daa77c40 # release-1.15.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

# Create the library target and named header set for beman_iterator_interface
add_library(beman_iterator_interface STATIC)
target_sources(
  beman_iterator_interface
  PUBLIC FILE_SET
         beman_iterator_interface_headers
         TYPE
         HEADERS
         BASE_DIRS
         src
         include
         "${PROJECT_BINARY_DIR}/include/")

add_subdirectory(src/beman/iterator_interface)
add_subdirectory(include/beman/iterator_interface)

add_subdirectory(examples)
if(ITERATOR_INTERFACE_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

# Coverage
configure_file("cmake/gcovr.cfg.in" gcovr.cfg @ONLY)

add_custom_target(
  process_coverage
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Running gcovr to process coverage results"
  COMMAND mkdir -p coverage
  COMMAND gcovr --config gcovr.cfg .)
