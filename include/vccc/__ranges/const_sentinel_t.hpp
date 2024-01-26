//
// Created by cosge on 2024-01-27.
//

#ifndef VCCC_RANGES_CONST_SENTINEL_T_HPP_
#define VCCC_RANGES_CONST_SENTINEL_T_HPP_

#include <type_traits>

#include "vccc/__ranges/cend.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool = range<R>::value, typename = void>
struct const_sentinel_impl {};

template<typename R>
struct const_sentinel_impl<R, true, void_t<decltype( ranges::cend(std::declval<R&>()) )>> {
  using type = decltype(ranges::cend(std::declval<R&>()));
};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename R>
struct const_sentinel : detail::const_sentinel_impl<R> {};

template<typename R>
using const_sentinel_t = typename const_sentinel<R>::type;

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CONST_SENTINEL_T_HPP_
