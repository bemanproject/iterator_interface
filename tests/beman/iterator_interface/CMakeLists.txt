# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

add_executable(beman.iterator_interface.tests)
target_sources(
    beman.iterator_interface.tests
    PRIVATE iterator_interface.test.cpp
)
target_link_libraries(
    beman.iterator_interface.tests
    PRIVATE beman::iterator_interface GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(beman.iterator_interface.tests)
