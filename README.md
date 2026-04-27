# beman.iterator_interface: iterator creation mechanisms

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable-next-line line-length -->
![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/bemanproject/iterator_interface/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/bemanproject/iterator_interface/actions/workflows/pre-commit-check.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/bemanproject/iterator_interface/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/iterator_interface?branch=main) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

**Implements**: [`std::iterator_interface` (P2727R4)](https://wg21.link/P2727R4)

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

## License

`beman.iterator_interface` is licensed under the Apache License v2.0 with LLVM Exceptions.

## Usage

Full runnable examples can be found in [`examples/`](examples/) - please check
[examples/README.md](./examples/README.md) for building the code on local setup
or on Compiler Explorer.

### Repeated Chars Iterator

The next code snippet shows iterator interface support added in
[`std::iterator_interface` (P2727R)](https://wg21.link/P2727R4): define a random
access iterator that iterates over a sequence of characters repeated indefinitely.

<!-- markdownlint-disable -->
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
<!-- markdownlint-enable -->

### Filter Integer Iterator

The next code snippet shows iterator interface support added in
[`std::iterator_interface` (P2727R4)](https://wg21.link/P2727R4): define a
forward iterator that iterates over a sequence of integers, skipping those that
do not satisfy a predicate.

<!-- markdownlint-disable -->
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
<!-- markdownlint-enable -->

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++20 standard or greater
* CMake 3.30 or later
* (Test Only) GoogleTest

You can disable building tests by setting CMake option `BEMAN_ITERATOR_INTERFACE_BUILD_TESTS` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler | Version | C++ Standards | Standard Library  |
|----------|---------|---------------|-------------------|
| GCC      | 14-13   | C++26-C++20   | libstdc++         |
| GCC      | 12      | C++23, C++20  | libstdc++         |
| Clang    | 22-19   | C++26-C++20   | libstdc++, libc++ |
| Clang    | 18      | C++26-C++20   | libc++            |
| Clang    | 18      | C++23, C++20  | libstdc++         |
| Clang    | 17      | C++26-C++20   | libc++            |
| Clang    | 17      | C++20         | libstdc++         |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

## Integrate beman.iterator_interface into your project

### Build

You can build iterator_interface using a CMake workflow preset:

```bash
cmake --workflow --preset gcc-release
```

To list available workflow presets, you can invoke:

```bash
cmake --list-presets=workflow
```

For details on building beman.iterator_interface without using a CMake preset, refer to the
[Contributing Guidelines](CONTRIBUTING.md).

### Installation

To install beman.iterator_interface globally after building with the `gcc-release` preset, you can
run:

```bash
sudo cmake --install build/gcc-release
```

Alternatively, to install to a prefix, for example `/opt/beman`, you can run:

```bash
sudo cmake --install build/gcc-release --prefix /opt/beman
```

This will generate the following directory structure:

```txt
/opt/beman
├── include
│   └── beman
│       └── iterator_interface
│           ├── iterator_interface.hpp
│           └── ...
└── lib
    └── cmake
        └── beman.iterator_interface
            ├── beman.iterator_interface-config-version.cmake
            ├── beman.iterator_interface-config.cmake
            └── beman.iterator_interface-targets.cmake
```

### CMake Configuration

If you installed beman.iterator_interface to a prefix, you can specify that prefix to your CMake
project using `CMAKE_PREFIX_PATH`; for example, `-DCMAKE_PREFIX_PATH=/opt/beman`.

You need to bring in the `beman.iterator_interface` package to define the `beman::iterator_interface` CMake
target:

```cmake
find_package(beman.iterator_interface REQUIRED)
```

You will then need to add `beman::iterator_interface` to the link libraries of any libraries or
executables that include `beman.iterator_interface` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::iterator_interface)
```

### Using beman.iterator_interface

To use `beman.iterator_interface` in your C++ project,
include an appropriate `beman.iterator_interface` header from your source code.

```c++
#include <beman/iterator_interface/iterator_interface.hpp>
```

> [!NOTE]
>
> `beman.iterator_interface` headers are to be included with the `beman/iterator_interface/` prefix.
> Altering include search paths to spell the include target another way (e.g.
> `#include <iterator_interface.hpp>`) is unsupported.
