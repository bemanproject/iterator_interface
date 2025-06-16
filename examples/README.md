# Examples for beman.iterator_interface

<!--
SPDX-License-Identifier: 2.0 license with LLVM exceptions
-->

List of usage examples for `beman.iterator_interface`.

## Examples

Check basic `beman.iterator_interface` library usages:

<!-- markdownlint-disable -->
* [P2727](https://wg21.link/P2727): local [./repeated_chars_iterator.cpp](./repeated_chars_iterator.cpp) or [repeated_chars_iterator.cpp@Compiler Explorer](https://godbolt.org/z/Yn9K15c9b)
* [P2727](https://wg21.link/P2727): local [./filter_int_iterator.cpp](./filter_int_iterator.cpp) or [filter_int_iterator.cpp@Compiler Explorer](https://godbolt.org/z/q6933enqe)
<!-- markdownlint-enable -->

## Local Build and Run

```shell
# build
$ cmake --workflow --preset gcc-debug

# run repeated_chars_iterator.cpp
$ .build/gcc-debug/examples/beman.iterator_interface.examples.repeated_chars_iterator
foofoof

# run filter_int_iterator.cpp
$ .build/gcc-debug/examples/beman.iterator_interface.examples.filter_int_iterator
2 4 10 200
```
