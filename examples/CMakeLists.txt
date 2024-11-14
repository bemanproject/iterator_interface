# cmake-format: off
# examples/CMakeLists.txt -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

# List of all buildable examples.
set(EXAMPLES repeated_chars_iterator)

foreach(example ${EXAMPLES})
  # Add example executable.
  add_executable(beman.iterator_interface.examples.${example} "")

  # Add example source file.
  target_sources(beman.iterator_interface.examples.${example}
                 PRIVATE ${example}.cpp)

  # Link example with the library.
  target_link_libraries(beman.iterator_interface.examples.${example}
                        beman::iterator_interface)

  # Install.
  install(
    TARGETS beman.iterator_interface.examples.${example}
    COMPONENT beman.iterator_interface.examples
    DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach()
