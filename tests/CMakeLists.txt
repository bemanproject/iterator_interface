# cmake-format: off
# tests/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

include(GoogleTest)

add_executable(beman.iterator_interface.tests)

target_sources(beman.iterator_interface.tests
               PRIVATE iterator_interface.test.cpp)

target_sources(
  beman.iterator_interface.tests
  PRIVATE FILE_SET beman_iterator_interface_tests_headers TYPE HEADERS)

target_link_libraries(
  beman.iterator_interface.tests PRIVATE beman::iterator_interface GTest::gtest
                                         GTest::gtest_main)

# Issue #32: Re-enable ASAN run CI/clang-19.
#
# Note: clang-19 + gtest_discover_tests + Asan setup causes errors on some
# platforms. Temporary switch to gtest_add_tests and skip some Asan checks.
# Change also applied for CI flows.
gtest_add_tests(TARGET beman.iterator_interface.tests "" AUTO)
