# cmake-format: off
# include/beman/iterator_interface/CMakeLists.txt -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

target_sources(
  beman.iterator_interface
  PUBLIC FILE_SET
         beman_iterator_interface_headers
         TYPE
         HEADERS
         BASE_DIRS
         ${PROJECT_BINARY_DIR}/include
         ${PROJECT_SOURCE_DIR}/include
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/iterator_interface.hpp
         ${CMAKE_CURRENT_SOURCE_DIR}/iterator_interface_access.hpp
         ${CMAKE_CURRENT_SOURCE_DIR}/detail/stl_interfaces/config.hpp
         ${CMAKE_CURRENT_SOURCE_DIR}/detail/stl_interfaces/fwd.hpp
         ${CMAKE_CURRENT_SOURCE_DIR}/detail/stl_interfaces/iterator_interface.hpp)
