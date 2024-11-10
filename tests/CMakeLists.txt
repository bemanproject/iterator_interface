# cmake-format: off
# src/beman/iterator_interface/tests/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

include(GoogleTest)

# Tests
add_executable(beman_iterator_interface_test)

target_sources(beman_iterator_interface_test
               PRIVATE iterator_interface.test.cpp)

target_sources(
  beman_iterator_interface_test
  PRIVATE FILE_SET beman_iterator_interface_test_headers TYPE HEADERS)

target_link_libraries(
  beman_iterator_interface_test PRIVATE beman::iterator_interface GTest::gtest
                                        GTest::gtest_main)

# Issue #32: Re-enable ASAN run CI/clang-19.
#
# Note: clang-19 + gtest_discover_tests + Asan setup causes errors on some
# platforms. Temporary switch to gtest_add_tests and skip some Asan checks.
# Change also applied for CI flows.
gtest_add_tests(TARGET beman_iterator_interface_test "" AUTO)
