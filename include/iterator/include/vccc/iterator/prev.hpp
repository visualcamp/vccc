//
// Created by cosge on 2023-12-25.
//

#ifndef VCCC_ITERATOR_PREV_HPP_
#define VCCC_ITERATOR_PREV_HPP_

#include <type_traits>

#include "vccc/core.hpp"
#include "vccc/iterator/bidirectional_iterator.hpp"
#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/iterator/advance.hpp"

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

/// @addtogroup iterator
/// @{
/// @addtogroup iterator_ranges_prev__func__operations ranges::prev
/// @brief decrement an iterator by a given distance or to a bound
/// @{


VCCC_INLINE_OR_STATIC constexpr detail::prev_niebloid prev{};

/// @}
/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ITERATOR_PREV_HPP_
