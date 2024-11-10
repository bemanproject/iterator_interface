# cmake-format: off
# include/beman/iterator_interface/CMakeLists.txt -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

target_sources(
  beman_iterator_interface
  PUBLIC FILE_SET
         beman_iterator_interface_headers
         TYPE
         HEADERS
         FILES
         iterator_interface.hpp
         iterator_interface_access.hpp
         detail/stl_interfaces/config.hpp
         detail/stl_interfaces/fwd.hpp
         detail/stl_interfaces/iterator_interface.hpp)
