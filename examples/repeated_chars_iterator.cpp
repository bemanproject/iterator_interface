// examples/repeated_chars_iterator.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// [P2727](https://wg21.link/P2727) example:
// An iterator that allows iterating over repetitions of a sequence of characters.

#include <beman/iterator_interface/iterator_interface.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string_view>

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

int main() {
    // Create a repeated_chars_iterator that iterates over the sequence "foo" repeated indefinitely:
    // "foofoofoofoofoofoo...". Will actually extract a prefix of the sequence and insert it into a std::string.
    constexpr const std::string_view target = "foo";
    constexpr const auto len = 7;                                        // Number of extracted characters from the sequence.

    // Create iterators that go over the sequence "foofoofoofoofoofoo...".
    repeated_chars_iterator it_first(target.data(), target.size(), 0);   // target.size() == 3 is the length of "foo", 0 is this iterator's position.
    repeated_chars_iterator it_last(target.data(), target.size(), len);  // Same as above, but now the iterator's position is 7.

    std::string extracted_result;
    std::copy(it_first, it_last, std::back_inserter(extracted_result));
    assert(extracted_result.size() == len);
    std::cout << extracted_result << "\n";                               // Expected output at STDOUT: "foofoof"

    return 0;
}

