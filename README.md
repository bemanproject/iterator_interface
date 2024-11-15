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

The intent is that the source and documentation are available for use by people implementing their iterator types.

The README itself is licesed with CC0 1.0 Universal. Copy the contents and incorporate in your own work as you see fit.

// SPDX-License-Identifier: CC0-1.0

## Examples

Full runable examples can be found in `examples/` - please check [./examples/README.md](./examples/README.md) for building the code on local setup or on Compiler Explorer.

### Repetead Chars Iterator

The next code snippet shows iterator interface support added in [`std::iterator_interface` (P2727R)](https://wg21.link/P2727R4): define a random access iterator that iterates over a sequence of characters repeated indefinitely.

```cpp
#include <beman/iterator_interface/iterator_interface.hpp>

// repeated_chars_iterator uses iterator_interface to define a random access iterator
// that iterates over a sequence of characters repeated indefinitely.
class repeated_chars_iterator
    : public beman::iterator_interface::ext_iterator_interface_compat<repeated_chars_iterator,
                                                                      std::random_access_iterator_tag,
                                                                      char,
                                                                      char> {
public:
    // Default constructor creates an end-of-range iterator.
    constexpr repeated_chars_iterator() : m_it_begin(nullptr), m_fixed_size(0), m_pos(0) {}

    // Constructor for the beginning of the sequence.
    constexpr repeated_chars_iterator(const char* it_begin, difference_type size, difference_type n)
        : m_it_begin(it_begin), m_fixed_size(size), m_pos(n) {}

    // Random access iterator requirements:
    constexpr auto operator*() const { return m_it_begin[m_pos % m_fixed_size]; }
    constexpr repeated_chars_iterator& operator+=(std::ptrdiff_t i) {
        m_pos += i;
        return *this;
    }
    constexpr auto operator-(repeated_chars_iterator other) const { return m_pos - other.m_pos; }

private:
    // Start of the sequence of characters.
    const char*  m_it_begin;

    // Number of characters in the sequence.
    difference_type m_fixed_size;

    // Current position in the sequence.
    difference_type m_pos;
};

// Create a repeated_chars_iterator that iterates over the sequence "foo" repeated indefinitely:
// "foofoofoofoofoofoo...". Will actually extract a prefix of the sequence and insert it into a std::string.
constexpr const std::string_view target = "foo";
constexpr const auto len = 7;                                        // Number of extracted characters from the sequence.

// Create iterators that iterate over the sequence "foofoofoofoofoofoo...".
repeated_chars_iterator it_first(target.data(), target.size(), 0);   // target.size() == 3 is the length of "foo", 0 is this iterator's position.
repeated_chars_iterator it_last(target.data(), target.size(), len);  // Same as above, but now the iterator's position is 7.

std::string extracted_result;
std::copy(it_first, it_last, std::back_inserter(extracted_result));
assert(extracted_result.size() == len);
std::cout << extracted_result << "\n";                               // Expected output at STDOUT: "foofoof"
```

### Filter Integer Iterator

The next code snippet shows iterator interface support added in [`std::iterator_interface` (P2727R4)](https://wg21.link/P2727R4): define a forward iterator that iterates over a sequence of integers, skipping those that do not satisfy a predicate.

```cpp
#include <beman/iterator_interface/iterator_interface.hpp>

// filtered_int_iterator uses iterator_interface to define a forward iterator
// that iterates over a sequence of integers, skipping those that do not satisfy a predicate.
template <typename Pred>
struct filtered_int_iterator
    : beman::iterator_interface::ext_iterator_interface_compat<filtered_int_iterator<Pred>,
                                                               std::forward_iterator_tag,
                                                               int> {
    // ...
};

// Create a filtered_int_iterator that iterates over the sequence {1, 2, 3, 4, 10, 11, 101, 200, 0},
// skipping odd numbers. 0 is not skipped, so it will be the last element in the sequence.
std::array a = {1, 2, 3, 4, 10, 11, 101, 200, 0};
filtered_int_iterator it{std::begin(a), std::end(a), [](int i) { return i % 2 == 0; }};

while (*it) {
    std::cout << *it << " ";
    ++it;
}
std::cout << "\n";
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
apt-get install                             \
    g++-14 gcc-14 gcc-13 g++-14             \
    clang-18 clang++-18 clang-17 clang++-17
```

### Instructions

#### Preset CMake Flows

This project strives to be as normal and simple a CMake project as possible. This build workflow in particular will
work, producing a static `beman.iterator_interface` library, ready to package:

```shell
$ cmake --workflow --preset gcc-debug
$ cmake --workflow --preset gcc-release
$ cmake --install .build/gcc-release --prefix /opt/beman.iterator_interface

$ tree /opt/beman.iterator_interface
├── bin      # examples (check: BEMAN_ITERATOR_INTERFACE_BUILD_EXAMPLES - default: ON)
│   ├── beman.iterator_interface.examples.filter_int_iterator
│   └── beman.iterator_interface.examples.repeated_chars_iterator
├── include  # public API
│   └── beman
│       └── iterator_interface
│           ├── config.hpp
│           ├── detail
│           │   └── stl_interfaces
│           │       ├── config.hpp
│           │       ├── fwd.hpp
│           │       └── iterator_interface.hpp
│           ├── iterator_interface.hpp
│           └── iterator_interface_access.hpp
└── lib     # actual library implementation
    └── libbeman.iterator_interface.a

8 directories, 9 files
```

<details>
<summary> Build beman.iterator_interface (verbose logs - gcc-debug) </summary>


This should build and run the tests with system GCC with the address and undefined behavior sanitizers enabled.

```shell
$ cmake --workflow --preset gcc-debug
Executing workflow step 1 of 3: configure preset "gcc-debug"

Preset CMake variables:

  CMAKE_BUILD_TYPE="Debug"
  CMAKE_CXX_COMPILER="g++"
  CMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fsanitize=undefined"
  CMAKE_CXX_STANDARD="17"

-- The CXX compiler identification is GNU 13.2.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test HAVE_DEDUCING_THIS
-- Performing Test HAVE_DEDUCING_THIS - Failed
-- The C compiler identification is GNU 13.2.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Configuring done (2.7s)
-- Generating done (0.0s)
-- Build files have been written to: /path/to/repo/.build/gcc-debug

Executing workflow step 2 of 3: build preset "gcc-debug"

[15/15] Linking CXX executable tests/beman/iterator_interface/beman.iterator_interface.tests

Executing workflow step 3 of 3: test preset "gcc-debug"

Test project /path/to/repo/.build/gcc-debug
    Start 1: IteratorTest.TestGTest
1/4 Test #1: IteratorTest.TestGTest ...........   Passed    0.01 sec
    Start 2: IteratorTest.TestRepeatedChars
2/4 Test #2: IteratorTest.TestRepeatedChars ...   Passed    0.01 sec
    Start 3: IteratorTest.TestFilteredIter
3/4 Test #3: IteratorTest.TestFilteredIter ....   Passed    0.01 sec
    Start 4: IteratorTest.OperatorArrow
4/4 Test #4: IteratorTest.OperatorArrow .......   Passed    0.01 sec

100% tests passed, 0 tests failed out of 4

Total Test time (real) =   0.04 sec
```

</details>


<details>
<summary> Install beman.iterator_interface (verbose logs - gcc-release) </summary>

```shell
# Build release.
$ cmake --workflow --preset gcc-release
Executing workflow step 1 of 3: configure preset "gcc-release"

Preset CMake variables:

  CMAKE_BUILD_TYPE="RelWithDebInfo"
  CMAKE_CXX_COMPILER="g++"
  CMAKE_CXX_FLAGS="-O3"
  CMAKE_CXX_STANDARD="17"

-- The CXX compiler identification is GNU 13.2.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test HAVE_DEDUCING_THIS
-- Performing Test HAVE_DEDUCING_THIS - Failed
-- The C compiler identification is GNU 13.2.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Configuring done (2.7s)
-- Generating done (0.0s)
-- Build files have been written to: /path/to/repo/.build/gcc-release

Executing workflow step 2 of 3: build preset "gcc-release"

[15/15] Linking CXX executable tests/beman/iterator_interface/beman.iterator_interface.tests

Executing workflow step 3 of 3: test preset "gcc-release"

Test project /path/to/repo/.build/gcc-release
    Start 1: IteratorTest.TestGTest
1/4 Test #1: IteratorTest.TestGTest ...........   Passed    0.00 sec
    Start 2: IteratorTest.TestRepeatedChars
2/4 Test #2: IteratorTest.TestRepeatedChars ...   Passed    0.00 sec
    Start 3: IteratorTest.TestFilteredIter
3/4 Test #3: IteratorTest.TestFilteredIter ....   Passed    0.00 sec
    Start 4: IteratorTest.OperatorArrow
4/4 Test #4: IteratorTest.OperatorArrow .......   Passed    0.00 sec

100% tests passed, 0 tests failed out of 4

# Install build artifacts from `build` directory into `/opt/beman.iterator_interface` path.
$ cmake --install .build/gcc-release --prefix /opt/beman.iterator_interface
-- Install configuration: "RelWithDebInfo"
-- Installing: /opt/beman.iterator_interface/lib/libbeman.iterator_interface.a
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/iterator_interface.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/iterator_interface_access.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/detail/stl_interfaces/config.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/detail/stl_interfaces/fwd.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/detail/stl_interfaces/iterator_interface.hpp
-- Installing: /opt/beman.iterator_interface/bin/beman.iterator_interface.examples.filter_int_iterator
-- Installing: /opt/beman.iterator_interface/bin/beman.iterator_interface.examples.repeated_chars_iterator
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/config.hpp


# Check tree.
$ tree /opt/beman.iterator_interface
/opt/beman.iterator_interface
├── bin
│   ├── beman.iterator_interface.examples.filter_int_iterator
│   └── beman.iterator_interface.examples.repeated_chars_iterator
├── include
│   └── beman
│       └── iterator_interface
│           ├── config.hpp
│           ├── detail
│           │   └── stl_interfaces
│           │       ├── config.hpp
│           │       ├── fwd.hpp
│           │       └── iterator_interface.hpp
│           ├── iterator_interface.hpp
│           └── iterator_interface_access.hpp
└── lib
    └── libbeman.iterator_interface.a

8 directories, 9 files

```

</details>

#### Custom CMake Flows

##### Default Build

CI current build and test flows:
```shell
# Configure.
$ cmake -G "Ninja Multi-Config" \
      -DCMAKE_CONFIGURATION_TYPES="RelWithDebInfo;Asan" \
      -DCMAKE_TOOLCHAIN_FILE=etc/clang-19-toolchain.cmake \
      -B .build -S .

# Build with config Asan.
$ cmake --build .build --config Asan --target all -- -k 0
# Build with config RelWithDebInfo.
$ cmake --build .build --config RelWithDebInfo  --target all -- -k 0

# Run tests with config Asan..
$ ctest --build-config Asan --output-on-failure --test-dir .build

# Install library (default: config set to RelWithDebInfo).
$ cmake --install .build/ --prefix /opt/beman.iterator_interface
-- Install configuration: "RelWithDebInfo"
-- Installing: /opt/beman.iterator_interface/lib/libbeman.iterator_interface.a
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/iterator_interface.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/iterator_interface_access.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/detail/stl_interfaces/config.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/detail/stl_interfaces/fwd.hpp
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/detail/stl_interfaces/iterator_interface.hpp
-- Installing: /opt/beman.iterator_interface/bin/beman.iterator_interface.examples.filter_int_iterator
-- Installing: /opt/beman.iterator_interface/bin/beman.iterator_interface.examples.repeated_chars_iterator
-- Installing: /opt/beman.iterator_interface/include/beman/iterator_interface/config.hpp
$ tree /opt/beman.iterator_interface
/opt/beman.iterator_interface
├── bin
│   ├── beman.iterator_interface.examples.filter_int_iterator
│   └── beman.iterator_interface.examples.repeated_chars_iterator
├── include
│   └── beman
│       └── iterator_interface
│           ├── config.hpp
│           ├── detail
│           │   └── stl_interfaces
│           │       ├── config.hpp
│           │       ├── fwd.hpp
│           │       └── iterator_interface.hpp
│           ├── iterator_interface.hpp
│           └── iterator_interface_access.hpp
└── lib
    └── libbeman.iterator_interface.a

8 directories, 9 files
```

<details>
<summary> Build beman.iterator_interface and tests (verbose logs) </summary>

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
</details>

##### Skip Tests

By default, we build and run tests. You can provide `-DBEMAN_ITERATOR_INTERFACE_BUILD_TESTING=OFF` and completely disable building tests:

```shell
# Configure.
$ cmake -G "Ninja Multi-Config" \
      -DCMAKE_CONFIGURATION_TYPES="RelWithDebInfo;Asan" \
      -DCMAKE_TOOLCHAIN_FILE=etc/clang-19-toolchain.cmake \
      -DBEMAN_ITERATOR_INTERFACE_BUILD_TESTING=OFF \
      -B .build -S .
```

##### Skip Examples

By default, we build and run tests. You can provide `-DBEMAN_ITERATOR_INTERFACE_BUILD_EXAMPLES=OFF` and completely disable building tests:

```shell
# Configure.
$ cmake -G "Ninja Multi-Config" \
      -DCMAKE_CONFIGURATION_TYPES="RelWithDebInfo;Asan" \
      -DCMAKE_TOOLCHAIN_FILE=etc/clang-19-toolchain.cmake \
      -DBEMAN_ITERATOR_INTERFACE_BUILD_EXAMPLES=OFF \
      -B .build -S .
```

