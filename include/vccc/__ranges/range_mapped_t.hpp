//
// Created by yonggyulee on 2024/01/13.
//

#ifndef VCCC_RANGES_RANGE_MAPPED_T_HPP
#define VCCC_RANGES_RANGE_MAPPED_T_HPP

#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool = input_range<R>::value /* false */, typename = void>
struct range_mapped_impl {};
template<typename R>
struct range_mapped_impl<R, true, void_t<typename range_value_t<R>::second_type>> {
 using type = typename range_value_t<R>::second_type;
};

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
