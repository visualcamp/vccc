//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_CONCEPTS_SWAPPABLE_WITH_HPP_
#define VCCC_CONCEPTS_SWAPPABLE_WITH_HPP_

#include <type_traits>

#include "vccc/__concepts/common_reference_with.hpp"
#include "vccc/__concepts/swap.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename U, typename = void>
struct swap_uref_test : std::false_type {};

template<typename T, typename U>
struct swap_uref_test<T, U, void_t<decltype( ranges::swap(std::declval<T>(), std::declval<U>()) )>> : std::true_type {};


template<typename T, typename U, bool = common_reference_with<T, U>::value /* false */>
struct swappable_with_impl : std::false_type {};

template<typename T, typename U>
struct swappable_with_impl<T, U, true>
    : conjunction<
          swap_uref_test<T, T>,
          swap_uref_test<U, U>,
          swap_uref_test<T, U>,
          swap_uref_test<U, T>
      > {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that a type can be swapped or that two types can be swapped with each other

The concept `%swappable_with<T, U>` specifies that expressions of the type and value category encoded by `T` and `U`
are swappable with each other.

@sa [std::swappable_with](https://en.cppreference.com/w/cpp/concepts/swappable)
 */
template<typename T, typename U>
struct swappable_with : detail::swappable_with_impl<T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_SWAPPABLE_WITH_HPP_
