//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ALGORITHM_RANGES_SWAP_RANGES_HPP
#define VCCC_ALGORITHM_RANGES_SWAP_RANGES_HPP

#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/in_in_result.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/indirectly_swappable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/iter_swap.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template< class I1, class I2 >
using swap_ranges_result = in_in_result<I1, I2>;

namespace detail {

struct swap_ranges_niebloid {
  template<typename I1, typename S1, typename I2, typename S2, std::enable_if_t<conjunction<
      input_iterator<I1>, sentinel_for<S1, I1>,
      input_iterator<I2>, sentinel_for<S2, I2>,
      indirectly_swappable<I1, I2>
  >::value, int> = 0>
  constexpr swap_ranges_result<I1, I2> operator()(I1 first1, S1 last1, I2 first2, S2 last2) const {
    for (; !(first1 == last1 || first2 == last2); ++first1, ++first2)
        ranges::iter_swap(first1, first2);
    return {std::move(first1), std::move(first2)};
  }

  template<typename R1, typename R2, std::enable_if_t<conjunction<
      input_range<R1>,
      input_range<R2>,
      indirectly_swappable<iterator_t<R1>, iterator_t<R2>>
  >::value, int> = 0>
  constexpr void operator()(R1&& r1, R2&& r2) const {
    return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::swap_ranges_niebloid swap_ranges{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_SWAP_RANGES_HPP
