//
// Created by yonggyulee on 1/24/24.
//

#ifndef VCCC_RANGES_NON_PROPAGATING_CACHE_HPP
#define VCCC_RANGES_NON_PROPAGATING_CACHE_HPP

#include <memory>
#include <type_traits>

#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/dereferenceable.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/optional.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, typename U, bool = dereferenceable<U>::value /* false */>
struct constructible_from_deref : std::false_type {};
template<typename T, typename U>
struct constructible_from_deref<T, U, true> : constructible_from<T, decltype(*std::declval<U>())> {};

} // namespace detail

/// @addtogroup ranges
/// @{

// https://eel.is/c++draft/range.adaptors#range.nonprop.cache
template<typename T>
class non_propagating_cache : private optional<T> {
  using base = optional<T>;
 public:
  static_assert(std::is_object<T>::value, "Constraints not satisfied");

  using base::emplace;
  using base::operator*;
  using base::operator->;
  using base::operator bool;
  using base::has_value;
  using base::value;
  using base::value_or;
  using base::and_then;
  using base::transform;
  using base::or_else;
  using base::swap;
  using base::reset;

  non_propagating_cache() = default;

  constexpr non_propagating_cache(const non_propagating_cache&) noexcept
      : base() {}
  constexpr non_propagating_cache(non_propagating_cache&& other) noexcept {
    other.reset();
  }

  constexpr non_propagating_cache& operator=(const non_propagating_cache& other) noexcept {
    if (vccc::addressof(other) != this) {
      reset();
    }
    return *this;
  }

  constexpr non_propagating_cache& operator=(non_propagating_cache&& other) noexcept {
    reset();
    other.reset();
    return *this;
  }

  template<typename I, std::enable_if_t<detail::constructible_from_deref<T, const I&>::value, int> = 0>
  constexpr T& emplace_deref(const I& i) {
    return emplace(*i);
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_NON_PROPAGATING_CACHE_HPP
