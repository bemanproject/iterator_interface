// include/beman/iterator_interface/iterator_interface.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef INCLUDED_ITERATOR_INTERFACE
#define INCLUDED_ITERATOR_INTERFACE

#include <beman/iterator_interface/config.hpp>
#include <beman/iterator_interface/iterator_interface_access.hpp>
#if !BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS()
#include <beman/iterator_interface/detail/stl_interfaces/iterator_interface.hpp>
#endif

#include <concepts>
#include <type_traits>
#include <iterator>

namespace beman {
namespace iterator_interface {

#if BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS()

using std::conditional_t;
using std::convertible_to;
using std::is_convertible_v;
using std::is_object_v;
using std::is_pointer_v;
using std::is_reference_v;
using std::is_same_v;
using std::output_iterator_tag;
using std::ptrdiff_t;
using std::remove_const_t;
using std::same_as;

using std::input_iterator_tag;
using std::strong_ordering;

template <typename T>
    requires is_object_v<T>
class proxy_arrow_result {
    T value_; // exposition only

  public:
    constexpr proxy_arrow_result(const T& value) noexcept(noexcept(T(value))) : value_(value) {}
    constexpr proxy_arrow_result(T&& value) noexcept(noexcept(T(std::move(value)))) : value_(std::move(value)) {}

    constexpr const T* operator->() const noexcept { return &value_; }
    constexpr T*       operator->() noexcept { return &value_; }
};

// [iterator.interface.tmpl], class template iterator_interface
template <class IteratorConcept,
          class ValueType,
          class Reference      = ValueType&,
          class Pointer        = ValueType*,
          class DifferenceType = ptrdiff_t>
class iterator_interface; // freestanding

template <class D1, class D2 = D1>
concept base_iter_3way = // exposition only
    requires(D1 d1, D2 d2) { iterator_interface_access::base(d1) <=> iterator_interface_access::base(d2); };

template <class D1, class D2 = D1>
concept iter_sub = requires(D1 d1, D2 d2) { // exposition only
    typename D1::difference_type;
    { d1 - d2 } -> convertible_to<typename D1::difference_type>;
};

template <class D>
constexpr auto operator+(D it, typename D::difference_type n)
    requires requires { it += n; }; // freestanding

template <class D>
constexpr auto operator+(typename D::difference_type n, D it)
    requires requires { it += n; }; // freestanding

template <class D1, class D2>
constexpr auto operator-(D1 lhs, D2 rhs) // freestanding
    requires requires { iterator_interface_access::base(lhs) - iterator_interface_access::base(rhs); };

template <class D>
constexpr auto operator-(D it, typename D::difference_type n) // freestanding
    requires requires { it += -n; };

template <class D1, class D2>
constexpr auto operator<=>(D1 lhs, D2 rhs) // freestanding
    requires base_iter_3way<D1, D2> || iter_sub<D1, D2>;

template <class D1, class D2>
constexpr bool operator<(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>; // freestanding

template <class D1, class D2>
constexpr bool operator<=(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>; // freestanding

template <class D1, class D2>
constexpr bool operator>(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>; // freestanding

template <class D1, class D2>
constexpr bool operator>=(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>; // freestanding

template <class D1, class D2>
concept base_iter_comparable = // exposition only
    requires(D1 d1, D2 d2) { iterator_interface_access::base(d1) == iterator_interface_access::base(d2); };

template <class D1, class D2>
constexpr bool operator==(D1 lhs, D2 rhs) // freestanding
    requires(is_convertible_v<D2, D1> || is_convertible_v<D1, D2>) && (base_iter_comparable<D1, D2> || iter_sub<D1>);

template <class IteratorConcept,
          class ValueType,
          class Reference      = ValueType,
          class DifferenceType = ptrdiff_t>
using proxy_iterator_interface = iterator_interface< // freestanding
    IteratorConcept,
    ValueType,
    Reference,
    proxy_arrow_result<Reference>,
    DifferenceType>;

template <class Pointer, class Reference, class T>
    requires is_pointer_v<Pointer>
decltype(auto) make_iterator_pointer(T&& value) // exposition only
{
    return std::addressof(value);
}

template <class Pointer, class Reference, class T>
auto make_iterator_pointer(T&& value) // exposition only
{
    return Pointer(std::forward<T>(value));
}

namespace detail {
template <typename IteratorConcept, typename ReferenceType, bool IsForwardIter>
struct iter_cat;

template <typename IteratorConcept, typename ReferenceType>
struct iter_cat<IteratorConcept, ReferenceType, false> {};

template <typename IteratorConcept, typename ReferenceType>
struct iter_cat<IteratorConcept, ReferenceType, true> {
    using iterator_category =
        std::conditional_t<
            !std::is_reference_v<ReferenceType>,
            std::input_iterator_tag,
            std::conditional_t<
                std::is_base_of_v<std::random_access_iterator_tag, IteratorConcept>,
                std::random_access_iterator_tag,
                std::conditional_t<
                    std::is_base_of_v<std::bidirectional_iterator_tag, IteratorConcept>,
                    std::bidirectional_iterator_tag,
                    std::forward_iterator_tag>>>;
};

} // namespace detail

template <class IteratorConcept, class ValueType, class Reference, class Pointer, class DifferenceType>
class iterator_interface
    : detail::iter_cat<IteratorConcept, Reference, std::derived_from<IteratorConcept, std::forward_iterator_tag>>
{
  public:
    using iterator_concept = IteratorConcept;
    using value_type      = remove_const_t<ValueType>;
    using reference       = Reference;
    using pointer         = conditional_t<is_same_v<iterator_concept, output_iterator_tag>, void, Pointer>;
    using difference_type = DifferenceType;

    constexpr decltype(auto) operator*(this auto&& self)
        requires requires { *iterator_interface_access::base(self); }
    {
        return *iterator_interface_access::base(self);
    }

    constexpr auto operator->(this auto&& self)
        requires(!same_as<pointer, void>) && is_reference_v<reference> && requires { *self; }
    {
        return make_iterator_pointer<pointer, reference>(*self);
    }

    constexpr decltype(auto) operator[](this const auto& self, difference_type n)
        requires requires { self + n; }
    {
        auto retval = self;
        retval      = retval + n;
        return *retval;
    }

    constexpr decltype(auto) operator++(this auto& self)
        requires requires { ++iterator_interface_access::base(self); } && (!requires { self += difference_type(1); })
    {
        ++iterator_interface_access::base(self);
        return self;
    }

    constexpr decltype(auto) operator++(this auto& self)
        requires requires { self += difference_type(1); }
    {
        return self += difference_type(1);
    }

    constexpr auto operator++(this auto& self, int)
        requires requires { ++self; }
    {
        if constexpr (is_same_v<IteratorConcept, input_iterator_tag>) {
            ++self;
        } else {
            auto retval = self;
            ++self;
            return retval;
        }
    }

    constexpr decltype(auto) operator+=(this auto& self, difference_type n)
        requires requires { iterator_interface_access::base(self) += n; }
    {
        iterator_interface_access::base(self) += n;
        return self;
    }

    constexpr decltype(auto) operator--(this auto& self)
        requires requires { --iterator_interface_access::base(self); } && (!requires { self += difference_type(1); })
    {
        --iterator_interface_access::base(self);
        return self;
    }

    constexpr decltype(auto) operator--(this auto& self)
        requires requires { self += -difference_type(1); }
    {
        return self += -difference_type(1);
    }

    constexpr auto operator--(this auto& self, int)
        requires requires { --self; }
    {
        auto retval = self;
        --self;
        return retval;
    }

    constexpr decltype(auto) operator-=(this auto& self, difference_type n)
        requires requires { self += -n; }
    {
        return self += -n;
    }
};

template <class D>
constexpr auto operator+(D it, typename D::difference_type n)
    requires requires { it += n; }
{
    return it += n;
}

template <class D>
constexpr auto operator+(typename D::difference_type n, D it)
    requires requires { it += n; }
{
    return it += n;
}

template <class D1, class D2>
constexpr auto operator-(D1 lhs, D2 rhs)
    requires requires { iterator_interface_access::base(lhs) - iterator_interface_access::base(rhs); }
{
    return iterator_interface_access::base(lhs) - iterator_interface_access::base(rhs);
}

template <class D>
constexpr auto operator-(D it, typename D::difference_type n)
    requires requires { it += -n; }
{
    return it += -n;
}

template <class D1, class D2>
constexpr auto operator<=>(D1 lhs, D2 rhs)
    requires base_iter_3way<D1, D2> || iter_sub<D1, D2>
{
    if constexpr (base_iter_3way<D1, D2>) {
        return iterator_interface_access::base(lhs) <=> iterator_interface_access::base(rhs);
    } else {
        using diff_type      = typename D1::difference_type;
        const diff_type diff = rhs - lhs;
        return diff < diff_type(0)   ? strong_ordering::less
               : diff_type(0) < diff ? strong_ordering::greater
                                     : strong_ordering::equal;
    }
}

template <class D1, class D2>
constexpr bool operator<(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>
{
    return (lhs - rhs) < typename D1::difference_type(0);
}

template <class D1, class D2>
constexpr bool operator<=(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>
{
    return (lhs - rhs) <= typename D1::difference_type(0);
}

template <class D1, class D2>
constexpr bool operator>(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>
{
    return (lhs - rhs) > typename D1::difference_type(0);
}

template <class D1, class D2>
constexpr bool operator>=(D1 lhs, D2 rhs)
    requires iter_sub<D1, D2>
{
    return (lhs - rhs) >= typename D1::difference_type(0);
}

template <class D1, class D2>
constexpr bool operator==(D1 lhs, D2 rhs)
    requires(is_convertible_v<D2, D1> || is_convertible_v<D1, D2>) && (base_iter_comparable<D1, D2> || iter_sub<D1>)
{
    if constexpr (base_iter_comparable<D1, D2>) {
        return iterator_interface_access::base(lhs) == iterator_interface_access::base(rhs);
    } else if constexpr (iter_sub<D1>) {
        return (lhs - rhs) == typename D1::difference_type(0);
    }
}

template <typename Derived,
          typename IteratorConcept,
          typename ValueType,
          typename Reference      = ValueType&,
          typename Pointer        = ValueType*,
          typename DifferenceType = std::ptrdiff_t>
using ext_iterator_interface_compat =
    iterator_interface<IteratorConcept, ValueType, Reference, Pointer, DifferenceType>;

#else

namespace detail {
template <class>
constexpr bool dependent_false = false; // workaround before CWG2518/P2593R1
}

template <class IteratorConcept,
          class ValueType,
          class Reference      = ValueType&,
          class Pointer        = ValueType*,
          class DifferenceType = std::ptrdiff_t>
class iterator_interface {
    static_assert(detail::dependent_false<IteratorConcept>,
                  "beman.iterator_interface was compiled with "
                  "BEMAN_ITERATOR_INTERFACE_USE_DEDUCING_THIS set to FALSE so "
                  "beman::iterator_interface::iterator_interface is not available. See "
                  "beman::iterator_interface::ext_iterator_interface_compat for a portable alternative.");
};

template <typename Derived,
          typename IteratorConcept,
          typename ValueType,
          typename Reference      = ValueType&,
          typename Pointer        = ValueType*,
          typename DifferenceType = std::ptrdiff_t>
using ext_iterator_interface_compat = detail::stl_interfaces::
    iterator_interface<Derived, IteratorConcept, ValueType, Reference, Pointer, DifferenceType>;

#endif

} // namespace iterator_interface
} // namespace beman

#endif
