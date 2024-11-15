// tests/beman/iterator_interface/iterator_interfaces.test.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/iterator_interface/iterator_interface.hpp>
#include <beman/iterator_interface/iterator_interface.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <ranges>

namespace beman {
namespace iterator_interface {

namespace {} // namespace

TEST(IteratorTest, TestGTest) { ASSERT_EQ(1, 1); }

#define CONSTEXPR_EXPECT_EQ(val1, val2)   \
    if (::std::is_constant_evaluated()) { \
        if (!((val1) == (val2))) {        \
            ::std::abort();               \
        }                                 \
    } else                                \
        EXPECT_EQ(val1, val2)

struct repeated_chars_iterator
    : ext_iterator_interface_compat<repeated_chars_iterator, std::random_access_iterator_tag, char, char> {
    constexpr repeated_chars_iterator() : first_(nullptr), size_(0), n_(0) {}
    constexpr repeated_chars_iterator(const char* first, difference_type size, difference_type n)
        : first_(first), size_(size), n_(n) {}

    constexpr char                     operator*() const { return first_[n_ % size_]; }
    constexpr repeated_chars_iterator& operator+=(std::ptrdiff_t i) {
        n_ += i;
        return *this;
    }
    constexpr auto operator-(repeated_chars_iterator other) const { return n_ - other.n_; }

  private:
    const char*     first_;
    difference_type size_;
    difference_type n_;
};

TEST(IteratorTest, TestRepeatedChars) {
    auto lambda = [&] {
        repeated_chars_iterator first("foo", 3, 0); // 3 is the length of "foo", 0 is this iterator's position.
        repeated_chars_iterator last("foo", 3, 7);  // Same as above, but now the iterator's position is 7.
        std::string             result;
        std::copy(first, last, std::back_inserter(result));
        CONSTEXPR_EXPECT_EQ(result, "foofoof");
    };

    static_assert((lambda(), true));
    lambda();
}

TEST(IteratorTest, TestDistance) {
    auto lambda = [&] {
        repeated_chars_iterator first("foo", 3, 0); // 3 is the length of "foo", 0 is this iterator's position.
        repeated_chars_iterator last("foo", 3, 3); // 3 is the length of "foo", 3 is this iterator's position.
        std::string             result;
        std::copy(first, last, std::back_inserter(result));
        CONSTEXPR_EXPECT_EQ(std::distance(first, last), 3);
    };

    static_assert((lambda(), true));
    lambda();    
}

TEST(IteratorTest, TestNext) {
    auto lambda = [&] {
        repeated_chars_iterator first("foo", 3, 0); // 3 is the length of "foo", 0 is this iterator's position.
        repeated_chars_iterator last("foo", 3, 3); // 3 is the length of "foo", 3 is this iterator's position.
        CONSTEXPR_EXPECT_EQ(std::next(first, 3), last);
    };

    static_assert((lambda(), true));
    lambda();    
}

TEST(IteratorTest, TestConcepts) {
    const auto test = [](auto&& it) {
        // The iterator type of it.
        using iterator = typename std::remove_reference_t<decltype(it)>;

        // Check std::contiguous_iterator concept.
        // Note: Check each sub-concept to get the less verbose error message first!
        static_assert(std::input_iterator<iterator>);
        static_assert(std::forward_iterator<iterator>);
        static_assert(std::bidirectional_iterator<iterator>);
        static_assert(std::random_access_iterator<iterator>);
    };

    repeated_chars_iterator first("foo", 3, 0); // 3 is the length of "foo", 0 is this iterator's position.
    test(first);
}

template <typename Pred>
struct filtered_int_iterator
    : ext_iterator_interface_compat<filtered_int_iterator<Pred>, std::forward_iterator_tag, int> {
    filtered_int_iterator() : it_(nullptr) {}
    filtered_int_iterator(int* it, int* last, Pred pred) : it_(it), last_(last), pred_(std::move(pred)) {
        it_ = std::find_if(it_, last_, pred_);
    }

    // A forward iterator based on iterator_interface usually requires
    // three user-defined operations.  since we are adapting an existing
    // iterator (an int *), we only need to define this one.  The others are
    // implemented by iterator_interface, using the underlying int *.
    filtered_int_iterator& operator++() {
        it_ = std::find_if(std::next(it_), last_, pred_);
        return *this;
    }

    // It is really common for iterator adaptors to have a base() member
    // function that returns the adapted iterator.
    int* base() const { return it_; }

  private:
    // Provide access to base_reference.
    friend iterator_interface_access;

    // These functions are picked up by iterator_interface, and used to
    // implement any operations that you don't define above.  They're not
    // called base() so that they do not collide with the base() member above.
#ifdef __cpp_explicit_this_parameter
    constexpr decltype(auto) base_reference(this auto& self) noexcept { return self.it_; }
#else
    constexpr decltype(auto) base_reference() noexcept { return it_; }
#endif

    int* it_;
    int* last_;
    Pred pred_;
};

TEST(IteratorTest, TestFilteredIter) {
    int                   a[] = {1, 2, 3, 4};
    filtered_int_iterator f{std::begin(a), std::end(a), [](int i) { return (i % 2) == 0; }};
    ASSERT_EQ(*f, 2);
    ++f;
    ASSERT_EQ(*f, 4);
}

struct ClassWithMemberFunction {
    int f() { return 3; }
};

struct AlwaysIterator
    : ext_iterator_interface_compat<AlwaysIterator, std::random_access_iterator_tag, ClassWithMemberFunction> {
    AlwaysIterator() : size_(0), n_(0) {}
    AlwaysIterator(difference_type size, difference_type n) : size_(size), n_(n) {}

    ClassWithMemberFunction operator*() const { return value_; }
    AlwaysIterator&         operator+=(std::ptrdiff_t i) {
        n_ += i;
        return *this;
    }
    auto operator-(AlwaysIterator other) const { return n_ - other.n_; }

  private:
    ClassWithMemberFunction value_;
    const char*             first_;
    difference_type         size_;
    difference_type         n_;
};

// Confirm operator-> works as expected
TEST(IteratorTest, OperatorArrow) {
    AlwaysIterator ai;
    ASSERT_EQ(ai->f(), 3);
}

struct dummy_input_iterator :
    public ext_iterator_interface_compat<
      dummy_input_iterator, std::input_iterator_tag, int, int const&, void, std::ptrdiff_t> {
  constexpr dummy_input_iterator() { }
  dummy_input_iterator(dummy_input_iterator const&) = delete;
  dummy_input_iterator& operator=(dummy_input_iterator const&) = delete;
  dummy_input_iterator(dummy_input_iterator&&) = default;
  dummy_input_iterator& operator=(dummy_input_iterator&&) = default;
  constexpr reference operator*() const {
    return foo;
  }
  constexpr dummy_input_iterator& operator++() {
    return *this;
  }
  constexpr void operator++(int) {}

  friend constexpr bool operator==(std::default_sentinel_t const&,
                                   dummy_input_iterator const&) {
    return true;
  }

  friend beman::iterator_interface::iterator_interface_access;

  int foo = 0;
};

static_assert(std::input_iterator<dummy_input_iterator>);
static_assert(!std::forward_iterator<dummy_input_iterator>);

} // namespace iterator_interface
} // namespace beman
