//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES_RANGE_VALUE_T_HPP
#define VCCC_RANGES_RANGE_VALUE_T_HPP

#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename I, bool = has_typename_type<iter_value<I>>::value /* true */>
struct range_value_impl_2 {
  using type = iter_value_t<I>;
};
template<typename I>
struct range_value_impl_2<I, false> {};

template<typename R, bool = has_typename_type<ranges::iterator<R>>::value /* true */>
struct range_value_impl_1 : range_value_impl_2<ranges::iterator_t<R>> {};
template<typename R>
struct range_value_impl_1<R, false> {};

} // namespace ranges

template<typename R>
struct range_value : detail::range_value_impl_1<R> {};

/// @addtogroup ranges
/// @{

/**
\brief Used to obtain the value type of the iterator type of range type R.

@sa [std::ranges::range_value_t](https://en.cppreference.com/w/cpp/ranges/iterator_t)
 */
template<typename R>
using range_value_t = typename range_value<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_RANGE_VALUE_T_HPP
