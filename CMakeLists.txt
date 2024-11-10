# CMakeLists.txt -*-CMake-*-
#
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.10)

project(beman_iterator_interface VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(FetchContent)
include(CompilerFeatureTest)

beman_iterator_check_deducing_this(COMPILER_SUPPORTS_DEDUCING_THIS)

option(BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS
  "Make use of deducing this. Turn this off for non-conforming compilers."
  ${COMPILER_SUPPORTS_DEDUCING_THIS})

option(BEMAN_ITERATOR_INTERFACE_ENABLE_TESTING "Build beman.iterator_interface tests" ${PROJECT_IS_TOP_LEVEL})

if(BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS AND NOT COMPILER_SUPPORTS_DEDUCING_THIS)
  message(WARNING "Building with deducing this support despite of the compiler's lack of support for it")
endif()

configure_file(
  "${PROJECT_SOURCE_DIR}/include/beman/iterator_interface/config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/beman/iterator_interface/config.hpp"
  @ONLY
)

if(BEMAN_ITERATOR_INTERFACE_ENABLE_TESTING)
  enable_testing()
endif()

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

add_subdirectory(extern)
add_subdirectory(src/beman/iterator_interface)
add_subdirectory(examples)

include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

install(
  EXPORT ${TARGETS_EXPORT_NAME}
  NAMESPACE ${CMAKE_PROJECT_NAME}
  DESTINATION ${INSTALL_CONFIGDIR}
  )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "cmake/Config.cmake.in"
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)
