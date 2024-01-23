//
// Created by yonggyulee on 2024/01/13.
//

#ifndef VCCC_RANGES_RANGE_MAPPED_T_HPP
#define VCCC_RANGES_RANGE_MAPPED_T_HPP

#include <tuple>
#include <type_traits>

#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, typename = void>
struct has_second_type : std::false_type {};
template<typename T>
struct has_second_type<T, void_t<typename T::second_type>> : std::true_type {};

template<typename T, typename = void>
struct has_second_tuple_element : std::false_type {};
template<typename T>
struct has_second_tuple_element<T, void_t<std::tuple_element_t<1, T>>> : std::true_type {};

template<typename RV, bool = has_second_type<RV>::value, bool = has_second_tuple_element<RV>::value>
struct guide_map {};
template<typename RV, bool v>
struct guide_map<RV, true, v> {
  using type = typename RV::second_type;
};
template<typename RV>
struct guide_map<RV, false, true> {
  using type = std::tuple_element_t<1, RV>;
};

template<typename R, bool = input_range<R>::value /* false */>
struct range_mapped_impl {};
template<typename R>
struct range_mapped_impl<R, true> : guide_map<range_value_t<R>> {};

} // namespace ranges

template<typename R>
struct range_mapped : detail::range_mapped_impl<R> {};

/// @addtogroup ranges
/// @{

/// @sa [std::ranges::range_mapped_t](https://en.cppreference.com/w/cpp/container/map/deduction_guides)
template<typename R>
using range_mapped_t = typename range_mapped<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_RANGE_MAPPED_T_HPP
