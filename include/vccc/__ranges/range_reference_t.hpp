//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES_RANGE_REFERENCE_T_HPP
#define VCCC_RANGES_RANGE_REFERENCE_T_HPP

#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename I, bool = has_typename_type<iter_reference<I>>::value /* true */>
struct range_reference_impl_2 {
  using type = iter_reference_t<I>;
};
template<typename I>
struct range_reference_impl_2<I, false> {};

template<typename R, bool = has_typename_type<iterator<R>>::value /* true */>
struct range_reference_impl_1 : range_reference_impl_2<iterator_t<R>> {};
template<typename R>
struct range_reference_impl_1<R, false> {};

} // namespace ranges

template<typename R>
struct range_reference : detail::range_reference_impl_1<R> {};

/// @addtogroup ranges
/// @{

/**
\brief Used to obtain the reference type of the iterator type of range type R.

@sa [std::ranges::range_reference_t](https://en.cppreference.com/w/cpp/ranges/iterator_t)
 */
template<typename R>
using range_reference_t = typename range_reference<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_RANGE_REFERENCE_T_HPP
