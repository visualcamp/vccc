//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_NUMERIC_SATURATE_CAST_HPP_
#define VCCC_NUMERIC_SATURATE_CAST_HPP_

#include <limits>
#include <type_traits>

#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

// high bit -> low bit
template<typename To, typename From, typename Any>
inline constexpr To saturate_cast_impl(From x, Any, Any, std::true_type /* digit<To>  <  digit<From> */) noexcept {
  if (x < static_cast<From>((std::numeric_limits<To>::min)()))
    return std::numeric_limits<To>::min();
  if (x > static_cast<From>((std::numeric_limits<To>::max)()))
    return std::numeric_limits<To>::max();
  return static_cast<To>(x);
}

// low bit -> high bit
template<typename To, typename From, typename Any>
inline constexpr To saturate_cast_impl(From x, Any, Any, std::false_type /* digit<To>  <  digit<From> */) noexcept {
  return static_cast<To>(x);
}

// unsigned -> signed
template<typename To, typename From, typename Any>
inline constexpr To saturate_cast_impl(From x, std::false_type /* signed */, std::true_type /* signed */, Any) noexcept {
  if (x > std::make_unsigned_t<To>((std::numeric_limits<To>::max)()))
    return (std::numeric_limits<To>::max)();
  return static_cast<To>(x);
}

// signed -> unsigned
template<typename To, typename From, typename Any>
inline constexpr To saturate_cast_impl(From x, std::true_type /* signed */, std::false_type /* signed */, Any) noexcept {
  if (x < 0)
    return 0;
  if (std::make_unsigned_t<From>(x) > (std::numeric_limits<To>::max)())
    return (std::numeric_limits<To>::max)();
  return static_cast<To>(x);
}

} // namespace detail

template<typename To, typename From, std::enable_if_t<conjunction<
    std::is_integral<To>,
    std::is_integral<From>
>::value, int> = 0>
inline constexpr To saturate_cast(From x) noexcept {
  return vccc::detail::saturate_cast_impl<To>(
      x,
      std::is_signed<From>{},
      std::is_signed<To>{},
      vccc::bool_constant<((std::numeric_limits<To>::digits) < (std::numeric_limits<From>::digits))>{}
  );
}

} // namespace vccc

#endif // VCCC_NUMERIC_SATURATE_CAST_HPP_
