# cmake-format: off
# examples/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

# List of all buildable examples.
set(EXAMPLES sample)

foreach(example ${EXAMPLES})
  # Add example executable.
  add_executable(${example} "")

  # Add example source file.
  target_sources(${example} PRIVATE ${example}.cpp)

  # Link example with the library.
  target_link_libraries(${example} beman::iterator_interface)

  # Install.
  install(
    TARGETS ${example}
    COMPONENT beman_iterator_interface_examples
    DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach()
