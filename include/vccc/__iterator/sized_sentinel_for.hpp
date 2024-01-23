//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_ITERATOR_SIZED_SENTINEL_FOR_HPP_
#define VCCC_ITERATOR_SIZED_SENTINEL_FOR_HPP_

#include <type_traits>

#include "vccc/__concepts/implicit_expression_check.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {

template<typename S, typename I>
struct disable_sized_sentinel_for : std::false_type {};

namespace detail {

template<typename S, typename I, typename = void, typename = void>
struct explicit_sized_sentinel_subtract_check : std::false_type {};

template<typename S, typename I>
struct explicit_sized_sentinel_subtract_check<
    S, I,
    void_t<decltype(std::declval<S>() - std::declval<I>())>,
    void_t<decltype(std::declval<I>() - std::declval<S>())>
  > : conjunction<
        same_as<decltype(std::declval<S>() - std::declval<I>()), iter_difference_t<I>>,
        same_as<decltype(std::declval<I>() - std::declval<S>()), iter_difference_t<I>>
      > {};

template<typename S, typename I>
struct sized_sentinel_requires : implicit_expression_check<explicit_sized_sentinel_subtract_check, const S&, const I&> {};

template<
    typename S,
    typename I,
    bool = conjunction<
          sentinel_for<S, I>,
          negation<disable_sized_sentinel_for<std::remove_cv_t<S>, std::remove_cv_t<I>>>,
          has_typename_type<iter_difference<I>>
        >::value
>
struct sized_sentinel_for_impl : std::false_type {};

template<typename S, typename I>
struct sized_sentinel_for_impl<S, I, true> : sized_sentinel_requires<S, I> {};

} // namespace detail


/// @addtogroup iterator
/// @{


/**
@brief specifies that the `-` operator can be applied to an iterator and a sentinel to calculate their difference in constant time

The `%sized_sentinel_for` concept specifies that an object of the iterator type `I` and an object of the sentinel type `S`
can be subtracted to compute the distance between them in constant time.

@sa [std::sized_sentinel_for](https://en.cppreference.com/w/cpp/iterator/sized_sentinel_for)
@sa ranges::sized_range
@sa ranges::size
 */

template<typename S, typename I>
struct sized_sentinel_for : detail::sized_sentinel_for_impl<S, I> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_SIZED_SENTINEL_FOR_HPP_
