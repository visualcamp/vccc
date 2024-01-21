//
// Created by cosge on 2024-01-13.
//

#ifndef VCCC_ITERATOR_NEXT_HPP_
#define VCCC_ITERATOR_NEXT_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/advance.hpp"
#include "vccc/__iterator/input_or_output_iterator.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/sentinel_for.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct next_niebloid {
  template<typename I>
  constexpr std::enable_if_t<input_or_output_iterator<I>::value, I>
  operator()(I i) const {
    ++i;
    return i;
  }

  template<typename I, std::enable_if_t<input_or_output_iterator<I>::value, int> = 0>
  constexpr I operator()(I i, iter_difference_t<I> n) const {
    ranges::advance(i, n);
    return i;
  }

  template<typename I, typename S, std::enable_if_t<conjunction<
      input_or_output_iterator<I>,
      sentinel_for<S, I>
  >::value, int> = 0>
  constexpr I operator()(I i, S bound) const {
    ranges::advance(i, bound);
    return i;
  }

  template<typename I, typename S, std::enable_if_t<conjunction<
      input_or_output_iterator<I>,
      sentinel_for<S, I>
  >::value, int> = 0>
  constexpr I operator()(I i, iter_difference_t<I> n, S bound) const {
    ranges::advance(i, n, bound);
    return i;
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup iterator
/// @{
/// @addtogroup iterator_ranges_next__func__operations ranges::next
/// @brief increment an iterator by a given distance or to a bound
///
/// returns the \f$ { n^{th}} \f$ successor of iterator `i`.
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::next_niebloid next{};

/// @}
/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_ITERATOR_NEXT_HPP_
