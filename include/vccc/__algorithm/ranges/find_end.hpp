//
// Created by YongGyu Lee on 4/12/24.
//

#ifndef VCCC_ALGORITHM_RANGES_FIND_END_HPP_
#define VCCC_ALGORITHM_RANGES_FIND_END_HPP_

#include <iterator>
#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/search.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirectly_comparable.hpp"
#include "vccc/__iterator/next.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_subrange_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/subrange.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct find_end_niebloid {
  template<typename I1, typename S1,
           typename I2, typename S2,
           typename Pred = ranges::equal_to,
           typename Proj1 = identity, typename Proj2 = identity,
           std::enable_if_t<conjunction<
               forward_iterator<I1>, sentinel_for<S1, I1>,
               forward_iterator<I2>, sentinel_for<S2, I2>,
               indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
           >::value, int> = 0>
  constexpr subrange<I1> operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    if (first2 == last2) {
      auto last_it = ranges::next(first1, last1);
      return {last_it, last_it};
    }

    auto result = ranges::search(std::move(first1),last1, first2, last2, pred, proj1, proj2);
    if (result.empty())
      return result;

    for (;;) {
      auto new_result = ranges::search(std::next(result.begin()), last1, first2, last2, pred, proj1, proj2);
      if (new_result.empty())
        return result;
      result = std::move(new_result);
    }
  }

  template<typename R1, typename R2,
           typename Pred = ranges::equal_to, typename Proj1 = identity, typename Proj2 = identity,
           std::enable_if_t<conjunction<
               forward_range<R1>, forward_range<R2>,
               indirectly_comparable<iterator_t<R1>, iterator_t<R2>, Pred, Proj1, Proj2>
           >::value, int> = 0>
  constexpr borrowed_subrange_t<R1> operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), std::move(pred), std::move(proj1), std::move(proj2));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::find_end_niebloid find_end{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_FIND_END_HPP_
