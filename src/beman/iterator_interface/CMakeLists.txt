# cmake-format: off
# src/beman/iterator_interface/CMakeLists.txt -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

# Ensure that iterator_interface gets compiled at least once.
target_sources(beman.iterator_interface PUBLIC iterator_interface.cpp)

# The library is empty -- exclude it
install(
  TARGETS beman.iterator_interface
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT beman_iterator_interface_library
          EXCLUDE_FROM_ALL)

install(
  TARGETS beman.iterator_interface FILE_SET beman_iterator_interface_headers
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT beman_iterator_interface_development)
