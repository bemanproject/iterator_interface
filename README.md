<!--
SPDX-License-Identifier: 2.0 license with LLVM exceptions
-->

# beman.iterator\_interface: iterator creation mechanisms

![CI Tests](https://github.com/beman-project/iterator_interface/actions/workflows/ci.yml/badge.svg)

**Implements**:
* [`std::iterator_interface` (P2727R4)](https://wg21.link/P2727R4)

Source is licensed with the Apache 2.0 license with LLVM exceptions

// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

Documentation and associated papers are licensed with the Creative Commons Attribution 4.0 International license.

// SPDX-License-Identifier: CC-BY-4.0

The intent is that the source and documentation are available for use by people implementing their own optional types as well as people using the optional presented here as-is.

The README itself is licesed with CC0 1.0 Universal. Copy the contents and incorporate in your own work as you see fit.

// SPDX-License-Identifier: CC0-1.0

## Examples

Full runable examples can be found in `examples/` - please check [./examples/README.md](./examples/README.md).

### TODO - first example

The next code snippet shows iterator interface support added in [`std::iterator_interface` (P2727R4)](https://wg21.link/P2727R4):

```cpp
TODO
```

### TODO - second example

The next code snippet shows iterator interface support added in [`std::iterator_interface` (P2727R4)](https://wg21.link/P2727R4):

```cpp
TODO
```

## How to Build

### Compiler Support

This is a modern C++ project which can be compiled with the latest C++ standards (**C++20 or later**).

Default build: `C++23`. Please check `etc/${compiler}-flags.cmake`.

### Dependencies

This project is mainly tested on `Ubuntu 22.04` and `Ubuntu 24.04`, but it should be as portable as CMake is. This
project has no C or C++ dependencies.

Build-time dependencies:

* `cmake`
* `ninja`, `make`, or another CMake-supported build system
  * CMake defaults to "Unix Makefiles" on POSIX systems

Example of installation on `Ubuntu 24.04`:

```shell
# Install tools:
apt-get install -y cmake make ninja-build

# Example of toolchains:
apt-get install                           \
  g++-14 gcc-14 gcc-13 g++-14             \
  clang-18 clang++-18 clang-17 clang++-17
```

### Instructions

#### Preset CMake Flows

This project strives to be as normal and simple a CMake project as possible. This build workflow in particular will
work, producing a static `beman.iterator_interface` library, ready to package:

```shell
$ cmake --workflow --list-presets
Available workflow presets:
  "system"
  "gcc-14"
  "gcc-13"
  "gcc-12"
  "clang-20"
  "clang-19"
  "clang-18"
  "clang-17"
  "clang-16

# Run examples:
$ cmake --workflow --preset gcc-14
Executing workflow step 1 of 3: configure preset "gcc-14"
...
-- Build files have been written to: /path/to/repo/.build/gcc-14

Executing workflow step 2 of 3: build preset "gcc-14"

ninja: no work to do.

Executing workflow step 3 of 3: test preset "gcc-14"

Test project /home/dariusn/git/Beman/iterator_interface/.build/gcc-14
    Start 1: IteratorTest.TestGTest
1/4 Test #1: IteratorTest.TestGTest ...........   Passed    0.00 sec
    Start 2: IteratorTest.TestRepeatedChars
2/4 Test #2: IteratorTest.TestRepeatedChars ...   Passed    0.00 sec
    Start 3: IteratorTest.TestFilteredIter
3/4 Test #3: IteratorTest.TestFilteredIter ....   Passed    0.00 sec
    Start 4: IteratorTest.OperatorArrow
4/4 Test #4: IteratorTest.OperatorArrow .......   Passed    0.00 sec
...

100% tests passed, 0 tests failed out of ...

Total Test time (real) =   0.09 sec
```

This should build and run the tests with GCC 14 with the address and undefined behavior sanitizers enabled.

#### Custom CMake Flows

##### Default Build

CI current build and test flows:

```shell
# Configure build: default build production code + tests (BEMAN_ITERATOR_INTERFACE_BUILD_TESTING=ON).
$ cmake -G "Ninja Multi-Config" \
      -DCMAKE_CONFIGURATION_TYPES="RelWithDebInfo;Asan" \
      -DCMAKE_TOOLCHAIN_FILE=etc/clang-19-toolchain.cmake \
      -B .build -S .
-- The CXX compiler identification is Clang 19.0.0
...
-- Build files have been written to: /path/to/repo/.build

# Build.
$ cmake --build .build --config Asan --target all -- -k 0
...
[12/12] Linking CXX executable ... # Note: 12 targets here (including tests).

# Run tests.
$ ctest --build-config Asan --output-on-failure --test-dir .build
Internal ctest changing into directory: /path/to/repo/.build
Test project /path/to/repo/.build
...
100% tests passed, 0 tests failed out of 82

Total Test time (real) =   0.67 sec
```

##### Build Production, but Skip Tests

By default, we build and run tests. You can provide `-DBEMAN_ITERATOR_INTERFACE_BUILD_TESTING=OFF` and completely disable building tests:

```shell
# Configure build: build production code, skip tests (BEMAN_ITERATOR_INTERFACE_BUILD_TESTING=OFF).
$ cmake -G "Ninja Multi-Config" \
      -DCMAKE_CONFIGURATION_TYPES="RelWithDebInfo;Asan" \
      -DCMAKE_TOOLCHAIN_FILE=etc/clang-19-toolchain.cmake \
      -DBEMAN_ITERATOR_INTERFACE_BUILD_TESTING=OFF \
      -B .build -S .
-- The CXX compiler identification is Clang 19.0.0
...
-- Build files have been written to: /path/to/repo/.build

# Build.
$ cmake --build .build --config Asan --target all -- -k 0
...
[5/5] Linking CXX executable ... # Note: 5 targets here (tests were not built).

# Check that tests are not built/installed.
$ ctest --build-config Asan --output-on-failure --test-dir .build
Internal ctest changing into directory: /path/to/beman/repo/.build
Test project /path/to/beman/repo/.build
No tests were found!!!
```
