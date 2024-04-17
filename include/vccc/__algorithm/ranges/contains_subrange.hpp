//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_ALGORITHM_RANGES_CONTAINS_SUBRANGE_HPP_
#define VCCC_ALGORITHM_RANGES_CONTAINS_SUBRANGE_HPP_

#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/search.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirectly_comparable.hpp"
#include "vccc/__iterator/indirect_binary_predicate.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

namespace detail {

struct contains_subrange_niebloid {
  template<
      typename I1, typename S1, typename I2, typename S2,
      typename Pred = ranges::equal_to, typename Proj1 = identity, typename Proj2 = identity,
      std::enable_if_t<conjunction<
          forward_iterator<I1>, sentinel_for<S1, I1>,
          forward_iterator<I2>, sentinel_for<S2, I2>,
          indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
  >::value, int> = 0>
  constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    return (first2 == last2) ||
           !ranges::search(first1, last1, first2, last2, pred, proj1, proj2).empty();
  }

  template<
      typename R1, typename R2,
      typename Pred = ranges::equal_to, typename Proj1 = identity, typename Proj2 = identity,
      std::enable_if_t<conjunction<
          forward_range<R1>,
          forward_range<R2>,
          indirectly_comparable<iterator_t<R1>, iterator_t<R2>, Pred, Proj1, Proj2>
  >::value, int> = 0>
  constexpr bool operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2),
                   std::move(pred), std::move(proj1), std::move(proj2));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::contains_subrange_niebloid contains_subrange{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_CONTAINS_SUBRANGE_HPP_
