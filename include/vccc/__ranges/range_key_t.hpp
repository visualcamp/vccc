//
// Created by yonggyulee on 2024/01/13.
//

#ifndef VCCC_RANGES_RANGE_KEY_T_HPP
#define VCCC_RANGES_RANGE_KEY_T_HPP

#include <type_traits>

#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool = input_range<R>::value /* false */, typename = void>
struct range_key_impl {};
template<typename R>
struct range_key_impl<R, true, void_t<typename range_value_t<R>::first_type>> {
 using type = std::remove_const_t< typename range_value_t<R>::first_type >;
};

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
