//
// Created by cosge on 2023-12-25.
//

#ifndef VCCC_ITERATOR_PREV_HPP_
#define VCCC_ITERATOR_PREV_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/bidirectional_iterator.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/advance.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct prev_niebloid {
  template<typename I>
  constexpr std::enable_if_t<bidirectional_iterator<I>::value, I>
  operator()(I i) const {
    --i;
    return i;
  }

  template<typename I>
  constexpr std::enable_if_t<bidirectional_iterator<I>::value, I>
  operator()(I i, iter_difference_t<I> n) const {
    ranges::advance(i, -n);
    return i;
  }


  template<typename I>
  constexpr std::enable_if_t<bidirectional_iterator<I>::value, I>
  operator()(I i, iter_difference_t<I> n, I bound) const {
    ranges::advance(i, -n, bound);
    return i;
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup iterator
/// @{
/// @addtogroup iterator_ranges_prev__func__operations ranges::prev
/// @brief decrement an iterator by a given distance or to a bound
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::prev_niebloid prev{};

/// @}
/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_ITERATOR_PREV_HPP_
