//
// Created by yonggyulee on 2024/01/13.
//

#ifndef VCCC_RANGES_RANGE_KEY_T_HPP
#define VCCC_RANGES_RANGE_KEY_T_HPP

#include <tuple>
#include <type_traits>

#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, typename = void>
struct has_first_type : std::false_type {};
template<typename T>
struct has_first_type<T, void_t<typename T::first_type>> : std::true_type {};

template<typename T, typename = void>
struct has_first_tuple_element : std::false_type {};
template<typename T>
struct has_first_tuple_element<T, void_t<std::tuple_element_t<0, T>>> : std::true_type {};

template<typename RV, bool = has_first_type<RV>::value, bool = has_first_tuple_element<RV>::value>
struct guide_key {};
template<typename RV, bool v>
struct guide_key<RV, true, v> {
  using type = std::remove_const_t< typename RV::first_type >;
};
template<typename RV>
struct guide_key<RV, false, true> {
  using type = std::remove_const_t< std::tuple_element_t<0, RV> >;
};

template<typename R, bool = input_range<R>::value /* false */>
struct range_key_impl {};
template<typename R>
struct range_key_impl<R, true> : guide_key<range_value_t<R>> {};

} // namespace ranges

template<typename R>
struct range_key : detail::range_key_impl<R> {};

/// @addtogroup ranges
/// @{

/// @sa [std::ranges::range_key_t](https://en.cppreference.com/w/cpp/container/map/deduction_guides)
template<typename R>
using range_key_t = typename range_key<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_RANGE_KEY_T_HPP
