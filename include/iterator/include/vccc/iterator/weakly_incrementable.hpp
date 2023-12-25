//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_ITERATOR_WEAKLY_INCREMENTABLE_HPP_
#define VCCC_ITERATOR_WEAKLY_INCREMENTABLE_HPP_

#include <limits>
#include <type_traits>

#include "vccc/concepts/same_as.hpp"
#include "vccc/concepts/movable.hpp"
#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/type_traits/detail/requires_helper.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_integer_like.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = vccc::detail::require<vccc::detail::iter_difference<T>>::value>
struct is_signed_integer_like_iter_difference : std::false_type {};

template<typename T>
struct is_signed_integer_like_iter_difference<T, true>
    : is_signed_integer_like<iter_difference_t<T>> {};

template<typename T, typename = void>
struct is_pre_incrementable : std::false_type {};

template<typename T>
struct is_pre_incrementable<T, void_t<decltype( ++std::declval<T&>() )>> : same_as<decltype( ++std::declval<T&>() ), T&> {};

template<typename T, typename = void>
struct is_post_incrementable : std::false_type {};

template<typename T>
struct is_post_incrementable<T, void_t<decltype( std::declval<T&>()++ )>> : std::true_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename I>
struct weakly_incrementable
    : conjunction<
        movable<I>,
        detail::is_signed_integer_like_iter_difference<I>,
        detail::is_pre_incrementable<I>,
        detail::is_post_incrementable<I>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_WEAKLY_INCREMENTABLE_HPP_
