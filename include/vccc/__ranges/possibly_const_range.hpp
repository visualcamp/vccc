//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_POSSIBLY_CONST_RANGE_HPP_
#define VCCC_RANGES_POSSIBLY_CONST_RANGE_HPP_

#include <type_traits>

#include "vccc/__ranges/constant_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/negation.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R>
constexpr auto& possibly_const_range_impl(R& r, std::true_type) noexcept {
  return const_cast<const R&>(r);
}

template<typename R>
constexpr auto& possibly_const_range_impl(R& r, std::false_type) noexcept {
  return r;
}

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename R, std::enable_if_t<input_range<R>::value, int> = 0>
constexpr auto& possibly_const_range(R& r) noexcept {
  return detail::possibly_const_range_impl(r, conjunction<constant_range<const R>, negation<constant_range<R>>>{});
}

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_POSSIBLY_CONST_RANGE_HPP_
