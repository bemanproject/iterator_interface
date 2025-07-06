// include/beman/iterator_interface/iterator_interface_access.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ITERATOR_INTERFACE_ACCESSS_HPP
#define ITERATOR_INTERFACE_ACCESSS_HPP

namespace beman {
namespace iterator_interface {
// [iterator.interface], iterator interface
// [iterator.interface.helpers], iterator interface helpers
struct iterator_interface_access; // freestanding

struct iterator_interface_access {
    template <typename D>
    static constexpr auto base(D& d) noexcept -> decltype(d.base_reference()) {
        return d.base_reference();
    }

    template <typename D>
    static constexpr auto base(const D& d) noexcept -> decltype(d.base_reference()) {
        return d.base_reference();
    }
};
} // namespace iterator_interface
} // namespace beman
#endif
