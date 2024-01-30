//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_ITERATOR_MOVE_SENTINEL_HPP
#define VCCC_ITERATOR_MOVE_SENTINEL_HPP

#include <iterator>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/semiregular.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"

namespace vccc {

/// @addtogroup iterator
/// @{

template<typename S>
class move_sentinel {
 public:
  static_assert(semiregular<S>::value, "Constraints not satisfied");

  constexpr move_sentinel() = default;

  constexpr explicit move_sentinel(S x)
      : last_(x) {}

  template<typename S2, std::enable_if_t<convertible_to<const S2&, S>::value, int> = 0>
  constexpr move_sentinel(const move_sentinel<S2>& other)
      : last_(other.last_) {}

  template<typename S2, std::enable_if_t<convertible_to<const S2&, S>::value, int> = 0>
  constexpr move_sentinel& operator=(const move_sentinel<S2>& other) {
    last_ = other.last_;
  }

  constexpr S base() const {
    return last_;
  }

  template<typename Iter, typename S2 = S, std::enable_if_t<sentinel_for<S2, Iter>::value, int> = 0>
  friend constexpr bool operator==(const std::move_iterator<Iter>& i, const move_sentinel& s) {
    return i.base() == s.last_;
  }

  template<typename Iter, typename S2 = S, std::enable_if_t<sentinel_for<S2, Iter>::value, int> = 0>
  friend constexpr bool operator!=(const std::move_iterator<Iter>& i, const move_sentinel& s) {
    return !(i == s);
  }

  template<typename Iter, typename S2 = S, std::enable_if_t<sentinel_for<S2, Iter>::value, int> = 0>
  friend constexpr bool operator==(const move_sentinel& s, const std::move_iterator<Iter>& i) {
    return i.base() == s.last_;
  }

  template<typename Iter, typename S2 = S, std::enable_if_t<sentinel_for<S2, Iter>::value, int> = 0>
  friend constexpr bool operator!=(const move_sentinel& s, const std::move_iterator<Iter>& i) {
    return !(i == s);
  }

  template<typename Iter, typename S2 = S, std::enable_if_t<sized_sentinel_for<S2, Iter>::value, int> = 0>
  friend constexpr iter_difference_t<Iter> operator-(const move_sentinel& s, const std::move_iterator<Iter>& i) {
    return s.last_ - i.base();
  }

  template<typename Iter, typename S2 = S, std::enable_if_t<sized_sentinel_for<S2, Iter>::value, int> = 0>
  friend constexpr iter_difference_t<Iter> operator-(const std::move_iterator<Iter>& i, const move_sentinel& s) {
    return i.base() - s.last_;
  }

 private:
  S last_;
};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_MOVE_SENTINEL_HPP
