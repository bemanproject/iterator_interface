// iterator_interface.t.cpp                                           -*-C++-*-
#include <beman/iterator_interface26/iterator_interface.hpp>
#include <beman/iterator_interface26/iterator_interface.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>

namespace beman {
namespace iterator_interface26 {

namespace {} // namespace

TEST(IteratorTest, TestGTest) { ASSERT_EQ(1, 1); }

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
    repeated_chars_iterator first("foo", 3, 0); // 3 is the length of "foo", 0 is this iterator's position.
    repeated_chars_iterator last("foo", 3, 7);  // Same as above, but now the iterator's position is 7.
    std::string             result;
    std::copy(first, last, std::back_inserter(result));
    ASSERT_EQ(result, "foofoof");
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

} // namespace iterator_interface26
} // namespace beman
