# cmake-format: off
# etc/gcc-12-toolchain.cmake -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/gcc-flags.cmake")

set(CMAKE_C_COMPILER gcc-12)
set(CMAKE_CXX_COMPILER g++-12)
