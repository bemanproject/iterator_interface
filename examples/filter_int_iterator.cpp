// examples/filter_int_iterator.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// [P2727](https://wg21.link/P2727) example:
// An iterator that allows filtering int elements of a sequence.
#include <beman/iterator_interface/iterator_interface.hpp>

#include <algorithm>
#include <array>
#include <iostream>

// filtered_int_iterator uses std::iterator_interface to define a forward iterator
// that iterates over a sequence of integers, skipping those that do not satisfy a predicate.
template <typename Pred>
struct filtered_int_iterator
    : beman::iterator_interface::ext_iterator_interface_compat<filtered_int_iterator<Pred>,
                                                               std::forward_iterator_tag,
                                                               int> {
    // Default constructor creates an end-of-range iterator.
    filtered_int_iterator() : m_it_begin(nullptr) {}

    // Constructor for the beginning of the sequence.
    filtered_int_iterator(int* it_begin, int* it_end, Pred pred) : m_it_begin(it_begin), m_it_end(it_end), m_pred(std::move(pred)) {
        m_it_begin = std::find_if(m_it_begin, m_it_end, m_pred);
    }

    // A forward iterator based on iterator_interface usually requires
    // three user-defined operations.  since we are adapting an existing
    // iterator (an int *), we only need to define this one.  The others are
    // implemented by iterator_interface, using the underlying int *.
    filtered_int_iterator& operator++() {
        m_it_begin = std::find_if(std::next(m_it_begin), m_it_end, m_pred);
        return *this;
    }

    // It is really common for iterator adaptors to have a base() member
    // function that returns the adapted iterator.
    int* base() const { return m_it_begin; }

  private:
    // Provide access to base_reference.
    friend beman::iterator_interface::iterator_interface_access;

    // Provide access to base_reference.
    constexpr auto base_reference() noexcept { return m_it_begin; }

    // Start of the sequence of integers.
    int* m_it_begin;

    // End of the sequence of integers.
    int* m_it_end;

    // Predicate that determines which integers to skip.
    Pred m_pred;
};

int main() {
    // Create a filtered_int_iterator that iterates over the sequence {1, 2, 3, 4, 10, 11, 101, 200, 0}, skipping odd numbers.
    // 0 is not skipped, so it will be the last element in the sequence.
    std::array a = {1, 2, 3, 4, 10, 11, 101, 200, 0};
    filtered_int_iterator it{std::begin(a), std::end(a), [](int i) { return i % 2 == 0; }};

    while (*it) {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << "\n";

    return 0;
}
