# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_EXAMPLES filter_int_iterator repeated_chars_iterator)
message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.iterator_interface.examples.${example})
    target_sources(
        beman.iterator_interface.examples.${example}
        PRIVATE ${example}.cpp
    )
    target_link_libraries(
        beman.iterator_interface.examples.${example}
        PRIVATE beman::iterator_interface
    )
endforeach()
