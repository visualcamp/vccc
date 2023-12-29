//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_RANGE_DIFFERENCE_T_HPP_
#define VCCC_RANGES_RANGE_DIFFERENCE_T_HPP_

#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename I, bool = has_typename_type<iter_difference<I>>::value /* true */>
struct range_difference_impl_2 {
  using type = iter_difference_t<I>;
};
template<typename I>
struct range_difference_impl_2<I, false> {};

template<typename R, bool = has_typename_type<ranges::iterator<R>>::value /* true */>
struct range_difference_impl_1 : range_difference_impl_2<ranges::iterator_t<R>> {};
template<typename R>
struct range_difference_impl_1<R, false> {};

} // namespace ranges

template<typename R>
struct range_difference : detail::range_difference_impl_1<R> {};


/// @addtogroup ranges
/// @{


/**
@brief Used to obtain the difference type of the iterator type of range type `R`.
*/

template<typename R>
using range_difference_t = typename range_difference<R>::type;

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_RANGE_DIFFERENCE_T_HPP_
