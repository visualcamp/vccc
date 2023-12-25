//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_CONCEPTS_SWAPPABLE_HPP_
#define VCCC_CONCEPTS_SWAPPABLE_HPP_

#include <type_traits>

#include "vccc/ranges/swap.hpp"
#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = is_referencable<T>::value, typename = void>
struct swappable_impl : std::false_type {};

template<typename T>
struct swappable_impl<T, true, void_t<decltype(ranges::swap(std::declval<T&>(), std::declval<T&>()))>>
    : std::true_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

template<typename T>
struct swappable : detail::swappable_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_SWAPPABLE_HPP_
