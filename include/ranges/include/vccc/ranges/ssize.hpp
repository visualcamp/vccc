//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES_SSIZE_HPP
#define VCCC_RANGES_SSIZE_HPP

#include <type_traits>
#include <utility>

#include "vccc/core/inline_or_static.hpp"
#include "vccc/ranges/size.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, bool = (sizeof(T) > sizeof(std::ptrdiff_t)) /* true */>
struct ssize_type {
  using type = T;
};
template<typename T>
struct ssize_type<T, false> {
  using type = std::ptrdiff_t;
};

template<typename T, typename = void>
struct is_ranges_size_callable : std::false_type {};
template<typename T>
struct is_ranges_size_callable<T, void_t<decltype( vccc::ranges::size(std::declval<T>()) )>> : std::true_type {};

struct ssize_niebloid {
  template<typename T, std::enable_if_t<is_ranges_size_callable<T&&>::value, int> = 0>
  constexpr auto operator()(T&& t) const {
    using size_type = decltype(ranges::size(std::forward<T>(t)));
    using unsigned_type = std::make_unsigned_t<size_type>;
    using R = typename ssize_type<unsigned_type>::type;
    return static_cast<R>(ranges::size(std::forward<T>(t)));
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup ranges
/// @{

/**
@brief	returns a signed integer equal to the size of a range

Returns the size of a range converted to a signed type.
 */

VCCC_INLINE_OR_STATIC constexpr detail::ssize_niebloid ssize{};

/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SSIZE_HPP
