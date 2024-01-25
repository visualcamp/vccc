//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_ALGORITHM_RANGES_MISMATCH_HPP
#define VCCC_ALGORITHM_RANGES_MISMATCH_HPP

#include <type_traits>
#include <functional>

#include "vccc/__algorithm/ranges/in_in_result.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirectly_comparable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

template<typename I1, typename I2>
using mismatch_result = in_in_result<I1, I2>;

namespace detail {

struct mismatch_niebloid {
  template<
      typename I1, typename S1,
      typename I2, typename S2,
      typename Pred = equal_to,
      typename Proj1 = identity, typename Proj2 = identity,
      std::enable_if_t<conjunction<
          input_iterator<I1>, sentinel_for<S1, I1>,
          input_iterator<I2>, sentinel_for<S2, I1>,
          indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
      >::value, int> = 0
  >
  constexpr mismatch_result<I1, I2>
  operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    for (; first1 != last1 && first2 != last2; ++first1, (void)++first2) {
      if (!vccc::invoke(pred, vccc::invoke(proj1, *first1), vccc::invoke(proj2, *first2))) {
        break;
      }
    }
    return {first1, first2};
  }

  template<typename R1, typename R2, typename Pred = equal_to, typename Proj1 = identity, typename Proj2 = identity>
  constexpr mismatch_result<borrowed_iterator_t<R1>, borrowed_iterator_t<R2>>
  operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    return (*this)(
        ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2),
        std::ref(pred), std::ref(proj1), std::ref(proj2));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::mismatch_niebloid mismatch{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_MISMATCH_HPP
